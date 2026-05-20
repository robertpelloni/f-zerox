#include <string.h>
#include <stdlib.h>
#include "pc/network/network.h"
#include "pc/configfile.h"
#include "pc/hal.h" // For HAL_GetTimeMillis
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Platform-independent socket includes
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>

    typedef int SOCKET;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

static SOCKET sSocket = INVALID_SOCKET;
static struct sockaddr_in sTargetAddr;

// Packet Types
#define PACKET_STATE 0
#define PACKET_EVENT 1

#define DEGTORAD 0.0174532925f

typedef struct {
    uint32_t magic; // 'FZXN'
    uint8_t type;
    uint8_t id;
    uint16_t seq;
    uint32_t timestamp; // Sender's local time

    // State Data
    float x, y, z;
    float yaw, pitch, roll;
    float velocity;
    bool boost;
} NetPacket;

#define MAX_NET_MACHINES 30

// Buffer for interpolation
typedef struct {
    float x, y, z;
    float yaw, pitch, roll;
    float velocity;
    uint32_t recvTimestamp; // Local time when received
    uint32_t sentTimestamp; // Remote time when sent
} NetState;

static NetState sNetBuffer[MAX_NET_MACHINES];
static bool sNetActive[MAX_NET_MACHINES];
static uint16_t sSeqNum = 0;

bool Net_Init(int port) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return false;
#endif

    sSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sSocket == INVALID_SOCKET) return false;

    // Non-blocking
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(sSocket, FIONBIO, &mode);
#else
    int flags = fcntl(sSocket, F_GETFL, 0);
    fcntl(sSocket, F_SETFL, flags | O_NONBLOCK);
#endif

    // Bind to receive
    struct sockaddr_in bindAddr;
    memset(&bindAddr, 0, sizeof(bindAddr));
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_port = htons(port);
    bindAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sSocket, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) < 0) {
        printf("Net: Failed to bind port %d. Network disabled.\n", port);
        closesocket(sSocket);
        sSocket = INVALID_SOCKET;
        return false;
    }

    // Setup Broadcast Target
    int broadcast = 1;
    if (setsockopt(sSocket, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast)) < 0) {
        printf("Net: Failed to enable broadcast.\n");
    }

    memset(&sTargetAddr, 0, sizeof(sTargetAddr));
    sTargetAddr.sin_family = AF_INET;
    sTargetAddr.sin_port = htons(port);
    sTargetAddr.sin_addr.s_addr = INADDR_BROADCAST;

    printf("Net: Initialized UDP socket on port %d (Broadcast).\n", port);
    return true;
}

void Net_BroadcastPos(Vehicle* v) {
    if (sSocket == INVALID_SOCKET) return;

    NetPacket pkt;
    pkt.magic = 0x4E585A46; // FZXN
    pkt.type = PACKET_STATE;

    static uint8_t myId = 0;
    if (myId == 0) myId = (uint8_t)(rand() % 255 + 1);
    pkt.id = myId;

    pkt.seq = sSeqNum++;
    pkt.timestamp = HAL_GetTimeMillis();

    pkt.x = v->x; pkt.y = v->y; pkt.z = v->z;
    pkt.yaw = v->yaw; pkt.pitch = v->pitch; pkt.roll = v->roll;
    pkt.velocity = v->velocity;
    pkt.boost = v->boost_active;

    sendto(sSocket, (const char*)&pkt, sizeof(pkt), 0, (struct sockaddr*)&sTargetAddr, sizeof(sTargetAddr));
}

void Net_Receive(void) {
    if (sSocket == INVALID_SOCKET) return;

    NetPacket pkt;
    struct sockaddr_in from;
    socklen_t fromLen = sizeof(from);

    while (true) {
        int len = recvfrom(sSocket, (char*)&pkt, sizeof(pkt), 0, (struct sockaddr*)&from, &fromLen);
        if (len < 0) break;

        if (len == sizeof(NetPacket) && pkt.magic == 0x4E585A46) {
            int slot = pkt.id % 29 + 1; // Basic hashing to array

            sNetBuffer[slot].x = pkt.x;
            sNetBuffer[slot].y = pkt.y;
            sNetBuffer[slot].z = pkt.z;
            sNetBuffer[slot].yaw = pkt.yaw;
            sNetBuffer[slot].pitch = pkt.pitch;
            sNetBuffer[slot].roll = pkt.roll;
            sNetBuffer[slot].velocity = pkt.velocity;
            sNetBuffer[slot].sentTimestamp = pkt.timestamp;
            sNetBuffer[slot].recvTimestamp = HAL_GetTimeMillis();
            sNetActive[slot] = true;
        }
    }
}

void Net_UpdateRemoteMachines(Vehicle* machines, int max_machines) {
    uint32_t now = HAL_GetTimeMillis();

    for (int i = 1; i < max_machines; i++) {
        if (sNetActive[i]) {
            // 1. Calculate Latency/Age of packet
            uint32_t ageMs = now - sNetBuffer[i].recvTimestamp;
            float ageSec = (float)ageMs / 1000.0f;

            // Timeout disconnect
            if (ageSec > 3.0f) {
                sNetActive[i] = false;
                machines[i].y = -10000.0f; // Hide
                continue;
            }

            // 2. Dead Reckoning: Predict where they are NOW
            // Forward Vector
            float radYaw = sNetBuffer[i].yaw * DEGTORAD;
            float radPitch = sNetBuffer[i].pitch * DEGTORAD;
            float fwdX = sinf(radYaw) * cosf(radPitch);
            float fwdY = sinf(radPitch);
            float fwdZ = -cosf(radYaw) * cosf(radPitch);

            float speed = sNetBuffer[i].velocity; // Units per frame? Assuming 60fps
            float speedPerSec = speed * 60.0f;

            // We want to account for the velocity damping (drag) and network jitter.
            // Instead of linear extrapolation to infinity, we cap the prediction time.
            float predAgeSec = ageSec;
            if (predAgeSec > 0.5f) predAgeSec = 0.5f; // Cap prediction to 500ms max

            float predX = sNetBuffer[i].x + (fwdX * speedPerSec * predAgeSec);
            float predY = sNetBuffer[i].y + (fwdY * speedPerSec * predAgeSec);
            float predZ = sNetBuffer[i].z + (fwdZ * speedPerSec * predAgeSec);

            // 3. Interpolate Towards Predicted Position
            // We use an exponential decay lerp for smoother visual arrival
            float dx = predX - machines[i].x;
            float dy = predY - machines[i].y;
            float dz = predZ - machines[i].z;
            float distSq = dx*dx + dy*dy + dz*dz;

            // k is the lerp factor. 1.0 means snap instantly.
            // If the predicted distance is very large (e.g., > 100 units), we snap harder.
            float k = 0.15f;
            if (distSq > 2500.0f) { // > 50 units away
                k = 0.5f;
            }
            if (distSq > 10000.0f) { // > 100 units away
                k = 1.0f; // Instant snap to correct extreme desync
            }

            machines[i].x += dx * k;
            machines[i].y += dy * k;
            machines[i].z += dz * k;

            // Shortest Path Angle Lerp for Yaw
            float diffYaw = sNetBuffer[i].yaw - machines[i].yaw;
            while (diffYaw > 180.0f) diffYaw -= 360.0f;
            while (diffYaw < -180.0f) diffYaw += 360.0f;
            machines[i].yaw += diffYaw * k;

            machines[i].pitch += (sNetBuffer[i].pitch - machines[i].pitch) * k;
            machines[i].roll += (sNetBuffer[i].roll - machines[i].roll) * k;

            machines[i].velocity = sNetBuffer[i].velocity;
        }
    }
}

void Net_Shutdown(void) {
    if (sSocket != INVALID_SOCKET) {
        closesocket(sSocket);
        sSocket = INVALID_SOCKET;
    }
#ifdef _WIN32
    WSACleanup();
#endif
}
