/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_PHONG_H
#define TRAYC_PHONG_H

#include <Trayc/CUDAfiles/common.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_math.h>
#include <Trayc/CUDAfiles/helper.h>
#include <Trayc/CUDAfiles/random.h>

rtDeclareVariable(rtObject, top_shadower, , );

rtDeclareVariable(int, max_depth, , );
rtDeclareVariable(int, shadow_samples, , );
rtDeclareVariable(int, diffuse_reflection_samples, , ) = 1;
rtDeclareVariable(int, ambient_occlusion_samples, , );
rtDeclareVariable(float, ao_sampling_radius, , );

rtDeclareVariable(float3, ambient_light_color, , );
rtDeclareVariable(float, importance_cutoff, , );

static __device__ __inline__ float ambientOcclusion(const float3 &hit_point, const float3 &normal, unsigned int &seed)
{
    if(ambient_occlusion_samples > 0)
    {
        float occlusion = 0.0f;
        for(int k = 0; k < ambient_occlusion_samples; ++k)
        {
            const float lambda = rnd(seed) * pi2;
            const float phi = acosf(2.0f * rnd(seed) - 1.0f);
            const float sinphi = sinf(phi);

            float3 sampleVector = make_float3(sinphi * cosf(lambda), sinphi * sinf(lambda), cosf(phi)) * ao_sampling_radius;
            if(dot(normal, sampleVector) < 0.0f)
                sampleVector = -sampleVector;

            PerRayData_shadow shadow_prd;
            shadow_prd.attenuation = 1.0f;
            const optix::Ray shadow_ray(hit_point, sampleVector, shadow_ray_type, scene_epsilon, 1.0f);
            rtTrace(top_shadower, shadow_ray, shadow_prd);
            occlusion += shadow_prd.attenuation;
        }

        return occlusion / (float)ambient_occlusion_samples;
    }
    return 1.0f;
}


static __device__ __inline__ void phongShadowed()
{
    prd_shadow.attenuation = 0.0f;
    rtTerminateRay();
}

static __device__ __inline__ void phongShade(const float3 &hit_point,
                                             const float3 &p_Ka,
                                             const float3 &p_Kd,
                                             const float3 &p_Ks,
                                             const float3 &ffnormal,
                                             float phong_exp,
                                             unsigned int &seed)
{
    float3 color = p_Ka * ambient_light_color;

    const int ctLights = lights.size();
    for(int i = 0; i < ctLights; ++i)
    {
        const BasicLight &light = lights[i];

        if(fmaxf(light.color) < importance_cutoff)
            continue;

        float attenuation = 0.0f;
        float Ldist = 0.0f;
        float3 L;

        if(light.flags & 1) // directional light?
        {
            attenuation = 1.0f;
            L = normalize(light.pos);
            Ldist = FLT_MAX;
        }
        else // point light or spotlight
        {
            L = light.pos - hit_point;
            Ldist = length(L);
            L *= (1.0f / Ldist);
            attenuation = 1.0f / (light.attenuation.x + light.attenuation.y * Ldist + light.attenuation.z * Ldist * Ldist);

            if(light.spot_cutoff <= 90.0f) // spotlight?
            {
                const float clampedCosine = max(0.0f, dot(-L, light.spot_direction));
                const float cutoffRadians = light.spot_cutoff * M_PIf / 180.0f;
                
                if(clampedCosine < cosf(cutoffRadians)) // outside of spotlight cone?
                    attenuation = 0.0f;
                else
                    attenuation = attenuation * powf(clampedCosine, light.spot_exponent);   
            }
        }

        const float nDl = dot(ffnormal, L);

        if(nDl > 0.0f && attenuation > importance_cutoff)
        {
            if(shadow_samples > 0 && (light.flags & 2))
            {
                if(shadow_samples == 1 || light.flags == LightFlags::SHADOWS_DIRECTIONAL)
                {
                    PerRayData_shadow shadow_prd;
                    shadow_prd.attenuation = 1.0f;
                    const optix::Ray sray(hit_point, L, shadow_ray_type, scene_epsilon, Ldist);
                    rtTrace(top_shadower, sray, shadow_prd);
                    attenuation *= shadow_prd.attenuation;
                }
                else
                {
                    float lighthit = 0.0f;
                    for(int k = 0; k < shadow_samples; ++k)
                    {
                        const float3 u = normalize(make_float3(L.y - L.z, -L.x, L.x));

                        const float r = rnd(seed) * light.radius;
                        const float t = rnd(seed) * pi2;
                        const float3 lightpoint =  r * (cosf(t) * u + sinf(t) * cross(L, u)) + light.pos;
                        const float3 Ldir = lightpoint - hit_point;

                        PerRayData_shadow shadow_prd;
                        shadow_prd.attenuation = 1.0f;
                        const optix::Ray shadow_ray(hit_point, normalize(Ldir), shadow_ray_type, scene_epsilon, length(Ldir));
                        rtTrace(top_shadower, shadow_ray, shadow_prd);
                        lighthit += shadow_prd.attenuation;
                    }
                    
                    attenuation *= (lighthit / (float)shadow_samples);
                }
            }

            if(attenuation > importance_cutoff)
            {
                const float3 light_color = light.color * attenuation;
                color += p_Kd * nDl * light_color;

                const float3 H = normalize(L - ray.direction);
                const float nDh = dot(ffnormal, H);
                if(nDh > 0.0f)
                    color += p_Ks * light_color * powf(nDh, phong_exp);
            }
        }
    }

    // pass the color back up the tree
    prd_radiance.result = color;
}

static __device__ __inline__ void phongReflect(const float3 &hit_point, const float3 &ffnormal, const float3 &reflectivity)
{
    if(fmaxf(reflectivity) > importance_cutoff)
    {
        const float3 r = schlick(-dot(ffnormal, ray.direction), reflectivity);
        const float importance = prd_radiance.importance * optix::luminance(r);

        //reflection ray
        if(importance > importance_cutoff && prd_radiance.depth < max_depth)
        {
            PerRayData_radiance refl_prd;
            refl_prd.importance = importance;
            refl_prd.depth = prd_radiance.depth+1;
            const float3 R = reflect(ray.direction, ffnormal);
            const optix::Ray refl_ray(hit_point, R, radiance_ray_type, scene_epsilon);
            rtTrace(top_object, refl_ray, refl_prd);
            prd_radiance.result += r * refl_prd.result;
            //TODO diffuse reflections
        }
    }
}

#endif
