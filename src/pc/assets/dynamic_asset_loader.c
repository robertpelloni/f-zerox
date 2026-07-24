#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "pc/assets/dynamic_asset_loader.h"

// Represents a mapping between a hardcoded ROM address and a dynamically loaded asset
typedef struct {
    uint32_t romAddress;
    void* loadedAsset;
} AssetMapEntry;

#define MAX_ASSET_MAPPINGS 1024
static AssetMapEntry sAssetMap[MAX_ASSET_MAPPINGS];
static int sAssetMapCount = 0;

// Register an asset mapping
void Asset_RegisterMapping(uint32_t address, void* asset) {
    if (sAssetMapCount < MAX_ASSET_MAPPINGS) {
        sAssetMap[sAssetMapCount].romAddress = address;
        sAssetMap[sAssetMapCount].loadedAsset = asset;
        sAssetMapCount++;
    } else {
        printf("Asset Loader: Mapping table full!\n");
    }
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

// Example usage to replace a hardcoded pointer in game logic
// Instead of:
// void* myAsset = (void*)0x80400000;
// We would do:
// void* myAsset = Asset_GetByAddress(0x80400000);
// Or preferably inject this mapping at the linker/virtual memory level.
