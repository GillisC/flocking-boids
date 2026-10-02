#include <cmath>

/* Dynamic linking */
#include <cstdlib>
#include <dlfcn.h>

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>

#include "boids.h"

internal void RenderFlock(const flock *Flock);

typedef flock * (CreateFlockFn)(u32, u32);
typedef void (UpdateFlockFn(flock *));
typedef void (FreeFlockFn(flock *));

int main() 
{
    InitWindow(1080, 720, "Flocking boids"); 
    SetTargetFPS(165);

    void *Handle = dlopen("./boids.so", RTLD_NOW);
    if (Handle == NULL)
    {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        return 1;
    }

    struct stat MetaData;
    if (stat("./boids.so", &MetaData) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    time_t ModifiedLast = MetaData.st_mtime;

    CreateFlockFn *CreateFlock = (CreateFlockFn *)dlsym(Handle, "CreateFlock");
    UpdateFlockFn *UpdateFlock = (UpdateFlockFn *)dlsym(Handle, "UpdateFlock");
    FreeFlockFn *FreeFlock = (FreeFlockFn *)dlsym(Handle, "FreeFlock");
    if (!CreateFlock)
    {
        fprintf(stderr, "dlsym failed: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    flock *Flock = CreateFlock(GetScreenWidth(), GetScreenHeight());

    while (!WindowShouldClose())
    {
        /* Reload the DLL if it has been updated */
        if (stat("boids.so", &MetaData) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }
        // fprintf(stderr, "Last modified: %ld, Current Modified: %ld\n", (long) ModifiedLast, (long) MetaData.st_mtime);

        if (MetaData.st_mtime != ModifiedLast)
        {
            /* Shared library has been updated, reload it */
            fprintf(stdout, "Detected that boids.so has been modified, reloading...\n");

            FreeFlock(Flock);
            dlclose(Handle);

            Handle = dlopen("./boids.so", RTLD_NOW);
            if (Handle == NULL)
            {
                fprintf(stderr, "dlopen failed: %s\n", dlerror());
                return 1;
            }

            CreateFlock = (CreateFlockFn *)dlsym(Handle, "CreateFlock");
            UpdateFlock = (UpdateFlockFn *)dlsym(Handle, "UpdateFlock");
            FreeFlock   = (FreeFlockFn *)dlsym(Handle, "FreeFlock");

            Flock = CreateFlock(GetScreenWidth(), GetScreenHeight());
            ModifiedLast = MetaData.st_mtime;
        }
        UpdateFlock(Flock);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        RenderFlock(Flock);
        EndDrawing();
    }

    CloseWindow();

    FreeFlock(Flock);
    dlclose(Handle);

    return 0;
}

internal void
RenderFlock(const flock *Flock)
{
    for (u32 BoidIndex = 0; BoidIndex < Flock->BoidCount; BoidIndex++)
    {
        boid Boid = Flock->Boids[BoidIndex];

        
        vec2 Centroid = Boid.Position;
        f32 L = 15.0f;
        f32 HalfAngle = DEG2RAD * 30.0f / 2;
        f32 HalfWidth = L * tanf(HalfAngle);

        vec2 B = {0,          (-2.0f * L / 3.0f)};
        vec2 A = {-HalfWidth, L / 3.0f};
        vec2 C = {HalfWidth,  L / 3.0f};

        f32 Theta = atan2f(Boid.Velocity.y, Boid.Velocity.x) + PI / 2.0f;

        A = Rotate(A, Theta);
        B = Rotate(B, Theta);
        C = Rotate(C, Theta);

        A = A + Centroid;
        B = B + Centroid;
        C = C + Centroid;

        DrawTriangle(
            {A.x, A.y},
            {C.x, C.y},
            {B.x, B.y},
            BLACK
        );
    }
}
