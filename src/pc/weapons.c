#include "pc/weapons.h"
#include "pc/gfx/fast3d.h"
#include "pc/physics.h"
#include "pc/gfx/particles.h"
#include <stdio.h>
#include <math.h>

#define MAX_PROJECTILES 64
static Projectile sProjectiles[MAX_PROJECTILES];

extern Vehicle gMachines[30]; // Access global machines

void Weapons_Init(void) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        sProjectiles[i].active = false;
    }
}

void Weapons_Fire(Vehicle* shooter, WeaponType type) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!sProjectiles[i].active) {
            sProjectiles[i].active = true;
            sProjectiles[i].type = type;
            sProjectiles[i].x = shooter->x;
            sProjectiles[i].y = shooter->y;
            sProjectiles[i].z = shooter->z;

            // Fire forward + offset
            float speed = 20.0f;
            if (type == WEAPON_MISSILE) speed = 30.0f;

            // Should add shooter velocity? Yes.
            // Estimate shooter velocity vector from Forward * Velocity?
            // Yes, assuming car is moving forward.

            sProjectiles[i].velocity[0] = sinf(shooter->yaw * 0.017453f) * speed;
            sProjectiles[i].velocity[1] = 0.0f * speed;
            sProjectiles[i].velocity[2] = -cosf(shooter->yaw * 0.017453f) * speed;

            printf("Weapon: Fired Type %d from %.1f, %.1f, %.1f\n", type, sProjectiles[i].x, sProjectiles[i].y, sProjectiles[i].z);
            break;
        }
    }
}

void Weapons_Update(void) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (sProjectiles[i].active) {
            sProjectiles[i].x += sProjectiles[i].velocity[0];
            sProjectiles[i].y += sProjectiles[i].velocity[1];
            sProjectiles[i].z += sProjectiles[i].velocity[2];

            // Collision with Machines
            for (int m = 0; m < 30; m++) {
                // Ignore self? Need owner ID.
                // For simplicity, just check distance.
                // Assuming machines have ~20 unit radius.
                float dx = sProjectiles[i].x - gMachines[m].x;
                float dy = sProjectiles[i].y - gMachines[m].y;
                float dz = sProjectiles[i].z - gMachines[m].z;

                if (dx*dx + dy*dy + dz*dz < 400.0f) {
                    // Hit!
                    printf("Weapon: Hit Machine %d!\n", m);
                    gMachines[m].energy -= 20.0f;
                    gMachines[m].velocity *= 0.8f; // Slow down

                    // Spawn explosion
                    for (int p=0; p<10; p++) {
                        Particles_Spawn(sProjectiles[i].x, sProjectiles[i].y, sProjectiles[i].z, 1.0f, 0.5f, 0.0f, 10.0f);
                    }

                    sProjectiles[i].active = false;
                    break;
                }
            }

            // Timeout / Bounds
            if (sProjectiles[i].z < -20000.0f || sProjectiles[i].z > 20000.0f) sProjectiles[i].active = false;
        }
    }
}

#include <SDL2/SDL_opengl.h>

void Weapons_Render(void) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (sProjectiles[i].active) {
            glPushMatrix();
            glTranslatef(sProjectiles[i].x, sProjectiles[i].y, sProjectiles[i].z);

            // Spin effect?
            static float spin = 0.0f;
            spin += 10.0f;
            glRotatef(spin, 0, 0, 1);

            // Draw a simple red diamond
            glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.5f, 0.0f);

            // Pyramid
            glVertex3f( 0,  5,  0);
            glVertex3f(-3,  0,  3);
            glVertex3f( 3,  0,  3);

            glVertex3f( 0,  5,  0);
            glVertex3f( 3,  0,  3);
            glVertex3f( 3,  0, -3);

            glVertex3f( 0,  5,  0);
            glVertex3f( 3,  0, -3);
            glVertex3f(-3,  0, -3);

            glVertex3f( 0,  5,  0);
            glVertex3f(-3,  0, -3);
            glVertex3f(-3,  0,  3);

            glEnd();

            glPopMatrix();
        }
    }
}
