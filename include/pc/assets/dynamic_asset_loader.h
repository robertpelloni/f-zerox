#ifndef DYNAMIC_ASSET_LOADER_H
#define DYNAMIC_ASSET_LOADER_H

#include <stdint.h>

void Asset_RegisterMapping(uint32_t address, void* asset);
void* Asset_GetByAddress(uint32_t address);

#endif
