#ifndef DYNAMIC_ASSET_LOADER_H
#define DYNAMIC_ASSET_LOADER_H

#include <stdint.h>
#include <stdbool.h>

void Asset_RegisterNamedMapping(const char* name, uint32_t address, void* asset);
void Asset_RegisterMapping(uint32_t address, void* asset);
bool Asset_UnregisterMapping(uint32_t address);
bool Asset_HasAddress(uint32_t address);
void* Asset_GetByAddress(uint32_t address);
void* Asset_GetByName(const char* name);
uint32_t Asset_GetAddressByName(const char* name);
int Asset_GetMappingCount(void);
uint32_t Asset_GetAddressByIndex(int index);
void* Asset_GetAssetByIndex(int index);
const char* Asset_GetNameByIndex(int index);
void Asset_ClearMappings(void);

#endif
