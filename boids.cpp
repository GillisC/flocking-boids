#include "boids.h"

#define PI 3.1415926535897932384626433832795f
#define DEG2RAD (PI / 180.0f)

u32 ActualBoidCount = 600;

static f32 MaxSpeed = 3.0f;
static f32 MaxForce = 0.2f;
static f32 ViewRadius = 25.0f;

f32 SeparateFactor = 100.0f;
f32 AlignFactor = 20.0f;
f32 CohesionFactor = 0.2f;


extern "C" flock *
CreateFlock(u32 ScreenWidth, u32 ScreenHeight)
{
    flock *Result = (flock *)malloc(sizeof(flock));

    Result->Boids = (boid *)calloc(ActualBoidCount, sizeof(boid));

    for (u32 BoidIndex = 0; BoidIndex < ActualBoidCount; BoidIndex++)
    {
        boid *Boid = &Result->Boids[BoidIndex];
        Boid->Position = RandomVec2(0, 1080, 0, 720);
        Boid->Velocity = RandomVec2(-2, 2, -2, 2);
    }

    Result->BoidCount = ActualBoidCount;
    Result->ScreenWidth = ScreenWidth;
    Result->ScreenHeight = ScreenHeight;

    return(Result);
}

extern "C" void 
UpdateFlock(flock *Flock)
{
    for (u32 CurrentBoidIndex = 0; CurrentBoidIndex < Flock->BoidCount; CurrentBoidIndex++)
    {
        boid *Current = Flock->Boids + CurrentBoidIndex;
        vec2 SeparateForceResult = {};
        vec2 AlignForceResult = {};
        vec2 CohesionForceResult = {};
        int NeighborCount = 0;

        for (u32 OtherBoidIndex = 0; OtherBoidIndex < Flock->BoidCount; OtherBoidIndex++)
        {
            boid *Other = Flock->Boids + OtherBoidIndex;
            f32 Dist = DistanceVec2(Current->Position, Other->Position);

            if (Dist > 0 && Dist < ViewRadius)
            {
                /* Separate */
                vec2 Diff = Current->Position - Other->Position;
                vec2 SteerForce = Normalize(Diff);
                SteerForce /= Dist; 
                SeparateForceResult += SteerForce;

                /* Align */
                AlignForceResult += Other->Velocity;

                /* Cohesion */
                CohesionForceResult += Other->Position;

                NeighborCount++;
            }
        }

        if (NeighborCount > 0)
        {
            SeparateForceResult /= NeighborCount;
            AlignForceResult /= NeighborCount;
            CohesionForceResult = CohesionForceResult / NeighborCount - Current->Position;

            /* Apply the force on the current boid */
            vec2 Result = 
                SeparateFactor * SeparateForceResult + 
                AlignFactor    * AlignForceResult + 
                CohesionFactor * CohesionForceResult;

            if (Length(Result) > 0)
            {
                Result = Normalize(Result);
                Result *= MaxSpeed;
                Result -= Current->Velocity;
                Result = Limit(Result, MaxForce);

                // F = ma Could play with mass here for each boid
                Current->Acceleration += Result;
            }
        }
    }

    for (u32 BoidIndex = 0; BoidIndex < Flock->BoidCount; BoidIndex++)
    {
        boid *Boid = Flock->Boids + BoidIndex;

        Boid->Velocity += Boid->Acceleration;
        Boid->Velocity = Limit(Boid->Velocity, MaxSpeed);
        Boid->Position += Boid->Velocity;

        f32 Margin = 25.0f;
        if (Boid->Position.x > Flock->ScreenWidth + Margin) Boid->Position.x = -Margin;
        if (Boid->Position.x < -Margin) Boid->Position.x = Flock->ScreenWidth + Margin;
        if (Boid->Position.y > Flock->ScreenHeight + Margin) Boid->Position.y = -Margin;
        if (Boid->Position.y < -Margin) Boid->Position.y = Flock->ScreenHeight + Margin;

        Boid->Acceleration = {};
    }
}

extern "C" void 
FreeFlock(flock *Flock)
{
    free(Flock->Boids);
    free(Flock);
}

