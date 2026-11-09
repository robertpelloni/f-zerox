#ifndef DYNAMIC_ASSET_LOADER_H
#define DYNAMIC_ASSET_LOADER_H

#include <stdint.h>
#include <stdbool.h>

void Asset_RegisterMapping(uint32_t address, void* asset);
bool Asset_UnregisterMapping(uint32_t address);
bool Asset_HasAddress(uint32_t address);
void* Asset_GetByAddress(uint32_t address);
int Asset_GetMappingCount(void);
void Asset_ClearMappings(void);

#endif
