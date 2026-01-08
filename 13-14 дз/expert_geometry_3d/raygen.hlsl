#include "raytracing.hlsl"

[shader("raygeneration")]
void RayGen()
{
    uint2 launchIndex = DispatchRaysIndex().xy;
    uint2 launchDim = DispatchRaysDimensions().xy;

    float2 crd = float2(launchIndex);
    float2 dims = float2(launchDim);

    float2 d = ((crd / dims) * 2.f - 1.f);
    RayDesc ray;
    ray.Origin = float3(0, 0, -2);
    ray.Direction = normalize(float3(d.x, -d.y, 1));
    ray.TMin = 0;
    ray.TMax = 100000;

    RayPayload payload;
    payload.color = float4(0, 0, 0, 0);
    TraceRay(Scene, RAY_FLAG_CULL_BACK_FACING_TRIANGLES, ~0, 0, 1, 0, ray, payload);

    gOutput[launchIndex] = payload.color;
}