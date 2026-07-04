#include <string.h>
#include <stdlib.h>
#include "pc/network/network.h"
#include "pc/configfile.h"
#include "pc/hal.h" // For HAL_GetTimeMillis
#include <stdio.h>
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
#define PACKET_HANDSHAKE 2

static uint8_t sMyPlayerId = 0;
static bool sLobbyConnected = false;

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
    float up[3];      // Normal vector
    float forward[3]; // Direction of travel
    float velocity;
    bool boost;
} NetPacket;

#define MAX_NET_MACHINES 30


#define HISTORY_BUFFER_SIZE 16

typedef struct {
    float x, y, z;
    float yaw, pitch, roll;
    float up[3];
    float forward[3];
    float velocity;
    uint32_t timestamp; // Sender's local time when this state was recorded
} NetStateEntry;

typedef struct {
    NetStateEntry history[HISTORY_BUFFER_SIZE];
    int head;
    int tail;
    int count;
    uint32_t lastRecvTime; // Local time
} NetBuffer;

static NetBuffer sNetRingBuffers[MAX_NET_MACHINES];

// Buffer for interpolation
typedef struct {
    float x, y, z;
    float yaw, pitch, roll;
    float up[3];
    float forward[3];
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

void Net_ConnectLobby(void) {
    if (sSocket == INVALID_SOCKET) return;

    // We don't have a server, so we broadcast a handshake request.
    // In a peer-to-peer setup, if someone responds with a conflict, we pick a new ID.
    // For simplicity, we just generate a random ID and broadcast it.
    // If we receive another handshake with this ID, we change ours.

    if (sMyPlayerId == 0) {
        sMyPlayerId = (uint8_t)(rand() % 29 + 1); // 1-29
    }

    NetPacket pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.magic = 0x4E585A46;
    pkt.type = PACKET_HANDSHAKE;
    pkt.id = sMyPlayerId;
    pkt.timestamp = HAL_GetTimeMillis();

    sendto(sSocket, (const char*)&pkt, sizeof(pkt), 0, (struct sockaddr*)&sTargetAddr, sizeof(sTargetAddr));

    // Optimistically assume we are connected
    sLobbyConnected = true;
    printf("Net: Sent Lobby Handshake. Claiming ID %d\n", sMyPlayerId);
}

void Net_BroadcastPos(Vehicle* v) {
    if (sSocket == INVALID_SOCKET) return;

    NetPacket pkt;
    pkt.magic = 0x4E585A46; // FZXN
    pkt.type = PACKET_STATE;

    if (!sLobbyConnected) {
        Net_ConnectLobby(); // Ensure we have an ID
    }
    pkt.id = sMyPlayerId;

    pkt.seq = sSeqNum++;
    pkt.timestamp = HAL_GetTimeMillis();

    pkt.x = v->x; pkt.y = v->y; pkt.z = v->z;
    pkt.yaw = v->yaw; pkt.pitch = v->pitch; pkt.roll = v->roll;

    pkt.up[0] = v->up[0]; pkt.up[1] = v->up[1]; pkt.up[2] = v->up[2];
    pkt.forward[0] = v->forward[0]; pkt.forward[1] = v->forward[1]; pkt.forward[2] = v->forward[2];

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
            if (pkt.type == PACKET_HANDSHAKE) {
                // Someone else is claiming an ID.
                if (pkt.id == sMyPlayerId) {
                    // Collision! The one with the smaller timestamp keeps it.
                    // This is a naive collision resolution for UDP P2P.
                    if (pkt.timestamp < HAL_GetTimeMillis()) {
                        printf("Net: ID Collision! Changing our ID.\n");
                        sMyPlayerId = (uint8_t)(rand() % 29 + 1);
                        Net_ConnectLobby(); // Re-broadcast our new claim
                    }
                }
                continue; // Handshake processed, skip state update
            }

            if (pkt.type != PACKET_STATE) continue;

            // Implement basic history buffer array logic for Rollback Netcode Interpolation
            // We can extend NetState to a cyclic buffer, but for now we'll stick to prediction and smoothing.


            int slot = pkt.id % 29 + 1; // Basic hashing to array

            // Add to ring buffer
            NetBuffer* buf = &sNetRingBuffers[slot];
            buf->history[buf->head].x = pkt.x;
            buf->history[buf->head].y = pkt.y;
            buf->history[buf->head].z = pkt.z;
            buf->history[buf->head].yaw = pkt.yaw;
            buf->history[buf->head].pitch = pkt.pitch;
            buf->history[buf->head].roll = pkt.roll;

            buf->history[buf->head].up[0] = pkt.up[0];
            buf->history[buf->head].up[1] = pkt.up[1];
            buf->history[buf->head].up[2] = pkt.up[2];
            buf->history[buf->head].forward[0] = pkt.forward[0];
            buf->history[buf->head].forward[1] = pkt.forward[1];
            buf->history[buf->head].forward[2] = pkt.forward[2];

            buf->history[buf->head].velocity = pkt.velocity;
            buf->history[buf->head].timestamp = pkt.timestamp;

            buf->head = (buf->head + 1) % HISTORY_BUFFER_SIZE;
            if (buf->count < HISTORY_BUFFER_SIZE) {
                buf->count++;
            } else {
                buf->tail = (buf->tail + 1) % HISTORY_BUFFER_SIZE; // Overwrite oldest
            }
            buf->lastRecvTime = HAL_GetTimeMillis();

            // Legacy fallback update
            sNetBuffer[slot].x = pkt.x;
            sNetBuffer[slot].y = pkt.y;
            sNetBuffer[slot].z = pkt.z;
            sNetBuffer[slot].yaw = pkt.yaw;
            sNetBuffer[slot].pitch = pkt.pitch;
            sNetBuffer[slot].roll = pkt.roll;

            sNetBuffer[slot].up[0] = pkt.up[0];
            sNetBuffer[slot].up[1] = pkt.up[1];
            sNetBuffer[slot].up[2] = pkt.up[2];
            sNetBuffer[slot].forward[0] = pkt.forward[0];
            sNetBuffer[slot].forward[1] = pkt.forward[1];
            sNetBuffer[slot].forward[2] = pkt.forward[2];

            sNetBuffer[slot].velocity = pkt.velocity;
            sNetBuffer[slot].sentTimestamp = pkt.timestamp;
            sNetBuffer[slot].recvTimestamp = HAL_GetTimeMillis();
            sNetActive[slot] = true;

        }
    }
}



void Net_UpdateRemoteMachines(Vehicle* machines, int max_machines) {
    uint32_t now = HAL_GetTimeMillis();

    // To implement proper rollback/interpolation we want to target a time in the past
    // corresponding to our input delay / rollback frames.
    uint32_t renderTime = now - (gConfig.input_delay * 16); // Assuming 60fps ~16ms per frame

    for (int i = 1; i < max_machines; i++) {
        if (sNetActive[i]) {
            NetBuffer* buf = &sNetRingBuffers[i];

            // 1. Calculate Latency/Age of packet
            uint32_t ageMs = now - buf->lastRecvTime;
            float ageSec = (float)ageMs / 1000.0f;

            // Timeout disconnect
            if (ageSec > 3.0f) {
                sNetActive[i] = false;
                machines[i].y = -10000.0f; // Hide
                continue;
            }

            // Interpolation Logic
            // We want to find the two states that bracket our `renderTime`
            if (buf->count >= 2) {
                NetStateEntry* newer = NULL;
                NetStateEntry* older = NULL;

                // Scan backwards through history (head-1 is newest)
                for (int j = 1; j <= buf->count; j++) {
                    int idx = (buf->head - j + HISTORY_BUFFER_SIZE) % HISTORY_BUFFER_SIZE;
                    NetStateEntry* entry = &buf->history[idx];

                    if (entry->timestamp <= renderTime) {
                        older = entry;
                        // The one just before it (newer)
                        if (j > 1) {
                            int newerIdx = (buf->head - (j - 1) + HISTORY_BUFFER_SIZE) % HISTORY_BUFFER_SIZE;
                            newer = &buf->history[newerIdx];
                        }
                        break;
                    }
                }

                if (older && newer) {
                    // We found our bracket, perform linear interpolation (lerp)
                    uint32_t timeDiff = newer->timestamp - older->timestamp;
                    if (timeDiff > 0) {
                        float t = (float)(renderTime - older->timestamp) / (float)timeDiff;

                        machines[i].x = older->x + (newer->x - older->x) * t;
                        machines[i].y = older->y + (newer->y - older->y) * t;
                        machines[i].z = older->z + (newer->z - older->z) * t;

                        // Shortest Path Angle Lerp for Yaw
                        float diffYaw = newer->yaw - older->yaw;
                        while (diffYaw > 180.0f) diffYaw -= 360.0f;
                        while (diffYaw < -180.0f) diffYaw += 360.0f;
                        machines[i].yaw = older->yaw + diffYaw * t;

                        // Same for pitch and roll if necessary
                        float diffPitch = newer->pitch - older->pitch;
                        while (diffPitch > 180.0f) diffPitch -= 360.0f;
                        while (diffPitch < -180.0f) diffPitch += 360.0f;
                        machines[i].pitch = older->pitch + diffPitch * t;

                        float diffRoll = newer->roll - older->roll;
                        while (diffRoll > 180.0f) diffRoll -= 360.0f;
                        while (diffRoll < -180.0f) diffRoll += 360.0f;
                        machines[i].roll = older->roll + diffRoll * t;

                        machines[i].up[0] = older->up[0] + (newer->up[0] - older->up[0]) * t;
                        machines[i].up[1] = older->up[1] + (newer->up[1] - older->up[1]) * t;
                        machines[i].up[2] = older->up[2] + (newer->up[2] - older->up[2]) * t;
                        machines[i].forward[0] = older->forward[0] + (newer->forward[0] - older->forward[0]) * t;
                        machines[i].forward[1] = older->forward[1] + (newer->forward[1] - older->forward[1]) * t;
                        machines[i].forward[2] = older->forward[2] + (newer->forward[2] - older->forward[2]) * t;

                        // Re-normalize up and forward vectors after interpolation
                        float upLen = sqrtf(machines[i].up[0]*machines[i].up[0] + machines[i].up[1]*machines[i].up[1] + machines[i].up[2]*machines[i].up[2]);
                        if (upLen > 0.0f) {
                            machines[i].up[0] /= upLen;
                            machines[i].up[1] /= upLen;
                            machines[i].up[2] /= upLen;
                        }

                        float fwdLen = sqrtf(machines[i].forward[0]*machines[i].forward[0] + machines[i].forward[1]*machines[i].forward[1] + machines[i].forward[2]*machines[i].forward[2]);
                        if (fwdLen > 0.0f) {
                            machines[i].forward[0] /= fwdLen;
                            machines[i].forward[1] /= fwdLen;
                            machines[i].forward[2] /= fwdLen;
                        }

                        machines[i].velocity = older->velocity + (newer->velocity - older->velocity) * t;
                        continue; // Done updating this machine via precise interpolation
                    }
                }
            }

            // Fallback to Dead Reckoning if we lack history or renderTime is too new/old
            // Forward Vector based on the latest state
            int latestIdx = (buf->head - 1 + HISTORY_BUFFER_SIZE) % HISTORY_BUFFER_SIZE;
            NetStateEntry* latest = &buf->history[latestIdx];

            float radYaw = latest->yaw * DEGTORAD;
            float radPitch = latest->pitch * DEGTORAD;
            float fwdX = sinf(radYaw) * cosf(radPitch);
            float fwdY = sinf(radPitch);
            float fwdZ = -cosf(radYaw) * cosf(radPitch);

            float speed = latest->velocity;
            float speedPerSec = speed * 60.0f;

            // Extrapolate from the time of the latest packet
            uint32_t extrapolationTimeMs = renderTime > latest->timestamp ? renderTime - latest->timestamp : 0;
            float predAgeSec = (float)extrapolationTimeMs / 1000.0f;

            if (predAgeSec > 0.5f) predAgeSec = 0.5f; // Cap prediction to 500ms max

            float predX = latest->x + (fwdX * speedPerSec * predAgeSec);
            float predY = latest->y + (fwdY * speedPerSec * predAgeSec);
            float predZ = latest->z + (fwdZ * speedPerSec * predAgeSec);

            // Interpolate smoothly towards the dead reckoned position
            float dx = predX - machines[i].x;
            float dy = predY - machines[i].y;
            float dz = predZ - machines[i].z;
            float distSq = dx*dx + dy*dy + dz*dz;

            float k = 0.15f;
            if (distSq > 2500.0f) k = 0.5f;
            if (distSq > 10000.0f) k = 1.0f;

            machines[i].x += dx * k;
            machines[i].y += dy * k;
            machines[i].z += dz * k;

            float diffYaw = latest->yaw - machines[i].yaw;
            while (diffYaw > 180.0f) diffYaw -= 360.0f;
            while (diffYaw < -180.0f) diffYaw += 360.0f;
            machines[i].yaw += diffYaw * k;

            float diffPitch = latest->pitch - machines[i].pitch;
            while (diffPitch > 180.0f) diffPitch -= 360.0f;
            while (diffPitch < -180.0f) diffPitch += 360.0f;
            machines[i].pitch += diffPitch * k;

            float diffRoll = latest->roll - machines[i].roll;
            while (diffRoll > 180.0f) diffRoll -= 360.0f;
            while (diffRoll < -180.0f) diffRoll += 360.0f;
            machines[i].roll += diffRoll * k;

            // Lerp vectors
            machines[i].up[0] += (latest->up[0] - machines[i].up[0]) * k;
            machines[i].up[1] += (latest->up[1] - machines[i].up[1]) * k;
            machines[i].up[2] += (latest->up[2] - machines[i].up[2]) * k;
            machines[i].forward[0] += (latest->forward[0] - machines[i].forward[0]) * k;
            machines[i].forward[1] += (latest->forward[1] - machines[i].forward[1]) * k;
            machines[i].forward[2] += (latest->forward[2] - machines[i].forward[2]) * k;

            // Re-normalize up and forward vectors after interpolation
            float upLen = sqrtf(machines[i].up[0]*machines[i].up[0] + machines[i].up[1]*machines[i].up[1] + machines[i].up[2]*machines[i].up[2]);
            if (upLen > 0.0f) {
                machines[i].up[0] /= upLen;
                machines[i].up[1] /= upLen;
                machines[i].up[2] /= upLen;
            }

            float fwdLen = sqrtf(machines[i].forward[0]*machines[i].forward[0] + machines[i].forward[1]*machines[i].forward[1] + machines[i].forward[2]*machines[i].forward[2]);
            if (fwdLen > 0.0f) {
                machines[i].forward[0] /= fwdLen;
                machines[i].forward[1] /= fwdLen;
                machines[i].forward[2] /= fwdLen;
            }

            machines[i].velocity = latest->velocity;
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
