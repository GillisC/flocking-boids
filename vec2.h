#pragma once

#include "base.h"
#include <cmath>
#include <cstdlib>

union vec2 {
    struct {
        f32 x, y;
    };

    f32 E[2];
};

inline vec2
Vec2(f32 X, f32 Y)
{
    vec2 Result = {};

    Result.x = X;
    Result.y = Y;

    return(Result);
}

inline vec2
Origin()
{
    vec2 Result = {};

    Result.x = 0.0;
    Result.y = 0.0;

    return(Result);
}

inline vec2
operator+(vec2 A, vec2 B)
{
    vec2 Result = {};

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return(Result);
}

inline vec2
operator-(vec2 A, vec2 B)
{
    vec2 Result = {};

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;

    return(Result);
}

inline vec2
operator-(vec2 A)
{
    vec2 Result = {};

    Result.x = -A.x;
    Result.y = -A.y;

    return(Result);
}

inline vec2
operator*(f32 t, vec2 A)
{
    vec2 Result = {};

    Result.x = A.x * t;
    Result.y = A.y * t;

    return(Result);
}

inline vec2
operator*(vec2 A, f32 t)
{
    vec2 Result = t * A;

    return(Result);
}

inline vec2
operator/(vec2 A, f32 t)
{
    vec2 Result = A * (1/t);

    return(Result);
}

inline vec2&
operator+=(vec2& A, const vec2& B)
{
    A.x += B.x;
    A.y += B.y;
    return (A);
}

inline vec2&
operator-=(vec2& A, vec2& B)
{
    A.x -= B.x;
    A.y -= B.y;
    return (A);
}

inline vec2&
operator*=(vec2& A, f32 t)
{
    A = t * A;

    return(A);
}

inline vec2&
operator*=(f32 t, vec2& A)
{
    A = t * A;

    return(A);
}

inline vec2&
operator/=(vec2& A, f32 t)
{
    A = A * (1/t);

    return(A);
}

inline f32
Dot(const vec2& A, const vec2& B)
{
    f32 Result = A.x*B.x + A.y*B.y;

    return(Result);
}

inline vec2
Hadamard(const vec2& A, const vec2& B)
{
    vec2 Result = {};

    Result.x = A.x * B.x;
    Result.y = A.y * B.y;

    return(Result);
}

inline f32
LengthSquared(const vec2& A)
{
    f32 Result = A.x*A.x + A.y*A.y;

    return(Result);
}

inline f32
Length(const vec2& A)
{
    f32 Result = sqrtf(LengthSquared(A));

    return(Result);
}

inline vec2
Normalize(const vec2& A)
{
    vec2 Result = {};
    if (Length(A) == 0) 
    {
        return(Result); 
    }

    Result = A / Length(A);

    return(Result);
}

inline f32
Clamp(const f32 A)
{
    if (A != A) return(0.0f);
    f32 Result = A > 1.0f ? 1.0f: A;
    Result = A < 0.0f ? 0.0f : A;
    return(Result);
}

inline vec2
Lerp(vec2 A, f32 Factor, vec2 B)
{
    vec2 Result = {};
    
    f32 DeltaX = B.x - A.x;
    f32 DeltaY = B.y - A.y;

    Result.x = A.x + DeltaX * Factor; 
    Result.y = A.y + DeltaY * Factor; 

    return(Result);
}

inline vec2
Rotate(vec2 A, f32 Radians)
{
    vec2 Result = {};

    Result.x = A.x * cosf(Radians) - A.y * sinf(Radians);
    Result.y = A.x * sinf(Radians) + A.y * cosf(Radians);

    return(Result);
}

inline f32
RandomF32(f32 Min, f32 Max)
{
    f32 Scale = rand() / (f32)RAND_MAX;
    f32 Result = Min + Scale * (Max - Min);

    return(Result);
}

inline vec2
RandomVec2(f32 MinX, f32 MaxX, f32 MinY, f32 MaxY)
{
    vec2 Result = {};

    Result.x = RandomF32(MinX, MaxX);
    Result.y = RandomF32(MinY, MaxY);

    return(Result);
}

inline vec2
Limit(vec2 A, f32 MagnitudeLimit)
{
    vec2 Result = A;
    if (Length(Result) <= MagnitudeLimit)
        return(Result);

    f32 ScalingFactor = MagnitudeLimit / Length(Result);
    Result *= ScalingFactor;

    return(Result);
}

inline f32
DistanceVec2(vec2 A, vec2 B)
{
    f32 Result = Length(B - A);
    return(Result);
}
