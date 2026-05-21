#ifndef NETWORK_H
#define NETWORK_H

#include "pc/physics.h"
#include <stdbool.h>

// Initialize networking (Bind UDP port)
bool Net_Init(int port);

// Handshake to establish ID
void Net_ConnectLobby(void);

// Send local vehicle state to server/peers
void Net_BroadcastPos(Vehicle* v);

// Receive pending packets and update remote vehicles
void Net_Receive(void);

// Apply received state to the game world (interpolation)
void Net_UpdateRemoteMachines(Vehicle* machines, int max_machines);

// Shutdown sockets
void Net_Shutdown(void);

#endif
