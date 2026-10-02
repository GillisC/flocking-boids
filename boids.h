#pragma once

#include "base.h"
#include "vec2.h"

struct boid {
    vec2 Position;
    vec2 Velocity;
    vec2 Acceleration;
};

struct flock {
    boid *Boids;
    u32 BoidCount;

    u32 ScreenWidth;
    u32 ScreenHeight;
};

extern "C" flock *CreateFlock(u32 ScreenWidth, u32 ScreenHeight);
extern "C" void UpdateFlock(flock *Flock);
extern "C" void FreeFlock(flock *Flock);

