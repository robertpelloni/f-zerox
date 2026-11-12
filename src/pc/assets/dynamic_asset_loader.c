#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "pc/assets/dynamic_asset_loader.h"

// Represents a mapping between a hardcoded ROM address and a dynamically loaded asset
typedef struct {
    uint32_t romAddress;
    char name[64];
    void* loadedAsset;
} AssetMapEntry;

#define MAX_ASSET_MAPPINGS 1024
static AssetMapEntry sAssetMap[MAX_ASSET_MAPPINGS];
static int sAssetMapCount = 0;

// Register a named asset mapping
void Asset_RegisterNamedMapping(const char* name, uint32_t address, void* asset) {
    for (int i = 0; i < sAssetMapCount; i++) {
        if (sAssetMap[i].romAddress == address || (name && name[0] && strcmp(sAssetMap[i].name, name) == 0)) {
            sAssetMap[i].romAddress = address;
            sAssetMap[i].loadedAsset = asset;
            if (name) {
                strncpy(sAssetMap[i].name, name, sizeof(sAssetMap[i].name) - 1);
                sAssetMap[i].name[sizeof(sAssetMap[i].name) - 1] = '\0';
            } else {
                sAssetMap[i].name[0] = '\0';
            }
            return;
        }
    }

    if (sAssetMapCount < MAX_ASSET_MAPPINGS) {
        sAssetMap[sAssetMapCount].romAddress = address;
        sAssetMap[sAssetMapCount].loadedAsset = asset;
        if (name) {
            strncpy(sAssetMap[sAssetMapCount].name, name, sizeof(sAssetMap[sAssetMapCount].name) - 1);
            sAssetMap[sAssetMapCount].name[sizeof(sAssetMap[sAssetMapCount].name) - 1] = '\0';
        } else {
            sAssetMap[sAssetMapCount].name[0] = '\0';
        }
        sAssetMapCount++;
    } else {
        printf("Asset Loader: Mapping table full!\n");
    }
}

// Register an asset mapping by address
void Asset_RegisterMapping(uint32_t address, void* asset) {
    Asset_RegisterNamedMapping("", address, asset);
}

// Retrieve an asset by its registered string name
void* Asset_GetByName(const char* name) {
    if (!name || !name[0]) {
        return NULL;
    }
    for (int i = 0; i < sAssetMapCount; i++) {
        if (strcmp(sAssetMap[i].name, name) == 0) {
            return sAssetMap[i].loadedAsset;
        }
    }
    return NULL;
}

// Retrieve an address by its registered string name
uint32_t Asset_GetAddressByName(const char* name) {
    if (!name || !name[0]) {
        return 0;
    }
    for (int i = 0; i < sAssetMapCount; i++) {
        if (strcmp(sAssetMap[i].name, name) == 0) {
            return sAssetMap[i].romAddress;
        }
    }
    return 0;
}

// Retrieve registered name of asset by index
const char* Asset_GetNameByIndex(int index) {
    if (index >= 0 && index < sAssetMapCount) {
        return sAssetMap[index].name;
    }
    return "";
}

// Unregister an asset mapping by address and compact table
bool Asset_UnregisterMapping(uint32_t address) {
    for (int i = 0; i < sAssetMapCount; i++) {
        if (sAssetMap[i].romAddress == address) {
            for (int j = i; j < sAssetMapCount - 1; j++) {
                sAssetMap[j] = sAssetMap[j + 1];
            }
            sAssetMapCount--;
            sAssetMap[sAssetMapCount].romAddress = 0;
            sAssetMap[sAssetMapCount].name[0] = '\0';
            sAssetMap[sAssetMapCount].loadedAsset = NULL;
            return true;
        }
    }
    return false;
}

// Check if an address has a registered mapping
bool Asset_HasAddress(uint32_t address) {
    for (int i = 0; i < sAssetMapCount; i++) {
        if (sAssetMap[i].romAddress == address) {
            return true;
        }
    }
    return false;
}

// Retrieve a mapped asset based on its original ROM address
void* Asset_GetByAddress(uint32_t address) {
    for (int i = 0; i < sAssetMapCount; i++) {
        if (sAssetMap[i].romAddress == address) {
            return sAssetMap[i].loadedAsset;
        }
    }
    return NULL;
}

int Asset_GetMappingCount(void) {
    return sAssetMapCount;
}

uint32_t Asset_GetAddressByIndex(int index) {
    if (index >= 0 && index < sAssetMapCount) {
        return sAssetMap[index].romAddress;
    }
    return 0;
}

void* Asset_GetAssetByIndex(int index) {
    if (index >= 0 && index < sAssetMapCount) {
        return sAssetMap[index].loadedAsset;
    }
    return NULL;
}

void Asset_ClearMappings(void) {
    memset(sAssetMap, 0, sizeof(sAssetMap));
    sAssetMapCount = 0;
}

// Example usage to replace a hardcoded pointer in game logic
// Instead of:
// void* myAsset = (void*)0x80400000;
// We would do:
// void* myAsset = Asset_GetByAddress(0x80400000);
// Or preferably inject this mapping at the linker/virtual memory level.
