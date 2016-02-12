
#ifndef EN_LIGHT_H
#define EN_LIGHT_H

#include <glm/glm.hpp>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    class Program;

    //---------- Light ----------//
    struct Light
    {
        Light(const glm::vec3 &intensity, bool isActive, LightType type);
        virtual ~Light();

        bool operator<(const Light* other) const;
        LightType GetType() const;
        // Params need to be transformed to view space
        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const = 0;


        glm::vec3 mIntensity;
        bool mIsActive;

    private:
        LightType mType;
    };


    //---------- DirectionalLight ----------//
    struct DirectionalLight : public Light
    {
        DirectionalLight(const glm::vec3 &intensity, const glm::vec3 &direction, bool isActive);

        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const override;

        glm::vec3 mDirection;
    };


    //---------- PointLight ----------//
    struct PointLight : public Light
    {
        PointLight(const glm::vec3 &intensity, 
            bool isActive,
            const glm::vec3 &attenuation,
            const glm::vec3 &position);

        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const override;

        //Constant, linear, quadratic
        glm::vec3 mAttenuation;
        //This is direction is the light is directional
        glm::vec3 mPosition;
    };


    //---------- SpotLight ----------//
    struct SpotLight : public Light
    {
        SpotLight(const glm::vec3 &intensity,
            bool isActive,
            const glm::vec3 &attenuation,
            const glm::vec3 &position,
            const glm::vec3 &spotDir,
            float spotCutoff,
            float spotExp);

        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const override;

        //Constant, linear, quadratic
        glm::vec3 mAttenuation;
        //This is direction is the light is directional
        glm::vec3 mPosition;
        // SpotLight specific
        glm::vec3 mSpotDirection;
        float mSpotCutoff;
        float mSpotExponent;
    };
}

#endif
