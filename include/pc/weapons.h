#ifndef WEAPONS_H
#define WEAPONS_H

#include "pc/physics.h"

typedef enum {
    WEAPON_NONE,
    WEAPON_MISSILE,
    WEAPON_MINE,
    WEAPON_SHIELD
} WeaponType;

typedef struct {
    bool active;
    float x, y, z;
    float velocity[3];
    WeaponType type;
} Projectile;

void Weapons_Init(void);
void Weapons_Fire(Vehicle* shooter, WeaponType type);
void Weapons_Update(void);
void Weapons_Render(void);

#endif
