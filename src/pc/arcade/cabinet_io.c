#include "pc/arcade/arcade_io.h"
#include <stdio.h>
#include <string.h>

// Mock Serial Port Implementation
// In a real build, we would use <termios.h> on Linux or <windows.h> on Windows

static bool sConnected = false;
static char sPortName[64];

bool Arcade_Init(const char* port) {
    snprintf(sPortName, 64, "%s", port);
    printf("Arcade IO: Attempting connection to %s...\n", port);

    // Simulate connection success
    sConnected = true;
    printf("Arcade IO: Connected to Motion Board.\n");
    return true;
}

void Arcade_SendMotion(float pitch, float roll, float heave) {
    if (!sConnected) return;

    // AX Cabinets expect specific byte sequences.
    // This is a placeholder protocol based on generic motion sim data.
    // Packet: [Header][Pitch][Roll][Heave][Checksum]

    uint8_t packet[8];
    packet[0] = 0xAX; // Header
    packet[1] = (int8_t)(pitch * 127.0f);
    packet[2] = (int8_t)(roll * 127.0f);
    packet[3] = (uint8_t)(heave * 255.0f);
    packet[4] = 0x00; // Checksum (placeholder)

    // In real implementation: write(fd, packet, 5);
    // For now, we log sparsely to avoid console spam
    static int logTimer = 0;
    if (logTimer++ > 60) {
        // printf("Arcade IO: Motion [P:%.2f R:%.2f H:%.2f]\n", pitch, roll, heave);
        logTimer = 0;
    }
}

void Arcade_Shutdown(void) {
    if (sConnected) {
        printf("Arcade IO: Disconnected.\n");
        sConnected = false;
    }
}
