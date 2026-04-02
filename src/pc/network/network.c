#include "pc/network/network.h"
#include "pc/configfile.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Platform-independent socket includes
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
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
static struct sockaddr_in sTargetAddr; // Where we send to

// Packet Types
#define PACKET_STATE 0
#define PACKET_EVENT 1

typedef struct {
    uint32_t magic; // 'FZXN'
    uint8_t type;
    uint8_t id;     // Player ID
    uint16_t seq;   // Sequence number

    // State Data
    float x, y, z;
    float yaw, pitch, roll;
    float velocity;
    float steer;
    bool boost;
} NetPacket;

#define MAX_NET_MACHINES 30
// Buffer for interpolation
typedef struct {
    float x, y, z;
    float yaw, pitch, roll;
    float velocity;
    uint32_t timestamp;
} NetState;

static NetState sNetBuffer[MAX_NET_MACHINES];
static bool sNetActive[MAX_NET_MACHINES];
static uint16_t sSeqNum = 0;

// Config: Client/Server mode? For now P2P Mesh or Broadcast.
// We broadcast state to everyone on the LAN.

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
    sTargetAddr.sin_addr.s_addr = INADDR_BROADCAST; // 255.255.255.255

    printf("Net: Initialized UDP socket on port %d (Broadcast).\n", port);
    return true;
}

void Net_BroadcastPos(Vehicle* v) {
    if (sSocket == INVALID_SOCKET) return;

    NetPacket pkt;
    pkt.magic = 0x4E585A46; // FZXN
    pkt.type = PACKET_STATE;
    // ID determination: We need a unique ID.
    // For now, use lower byte of IP? Or random ID generated at startup?
    // Let's rely on config or random.
    // Use configfile nick hash?
    static uint8_t myId = 0;
    if (myId == 0) myId = (uint8_t)(rand() % 255 + 1); // Random 1-255
    pkt.id = myId;

    pkt.seq = sSeqNum++;
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
            // Ignore self loopback (if broadcast goes to self)
            // But we don't know our ID until we send?
            // Actually broadcast usually reflects.
            // We need to filter based on ID.

            // Map ID to slot index (1..29)
            // Simple hash: ID % 29 + 1
            int slot = pkt.id % 29 + 1;

            sNetBuffer[slot].x = pkt.x;
            sNetBuffer[slot].y = pkt.y;
            sNetBuffer[slot].z = pkt.z;
            sNetBuffer[slot].yaw = pkt.yaw;
            sNetBuffer[slot].pitch = pkt.pitch;
            sNetBuffer[slot].roll = pkt.roll;
            sNetBuffer[slot].velocity = pkt.velocity;
            sNetActive[slot] = true;
        }
    }
}

void Net_UpdateRemoteMachines(Vehicle* machines, int max_machines) {
    // Apply buffered states
    // In a real system we'd use timestamps and interpolation buffers.
    // Here we just snap/lerp to latest packet.

    for (int i = 1; i < max_machines; i++) {
        if (sNetActive[i]) {
            // Lerp 20%
            float k = 0.2f;
            machines[i].x += (sNetBuffer[i].x - machines[i].x) * k;
            machines[i].y += (sNetBuffer[i].y - machines[i].y) * k;
            machines[i].z += (sNetBuffer[i].z - machines[i].z) * k;

            // Angle lerp needs shortest path
            // For now simple lerp
            machines[i].yaw += (sNetBuffer[i].yaw - machines[i].yaw) * k;
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
