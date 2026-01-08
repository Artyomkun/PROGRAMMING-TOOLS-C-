#ifndef RAYTRACING_HLSL
#define RAYTRACING_HLSL

struct RayPayload
{
    float4 color;
};

RaytracingAccelerationStructure Scene : register(t0);
RWTexture2D<float4> gOutput : register(u0);

#endif // RAYTRACING_HLSL