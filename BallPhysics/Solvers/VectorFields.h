/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_VECTORFIELDS_H
#define BP_VECTORFIELDS_H

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>


struct VectorField
{
    virtual glm::vec3 F(const glm::vec3 &p) const = 0;
};

struct Gravity : public VectorField
{
    Gravity(const glm::vec3 &gravity = glm::vec3(0.0f, -9.81f, 0.0f))
        : mGravity(gravity)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const override
    {
        return mGravity;
    }
private:
    glm::vec3 mGravity;
};

struct SphereAttractor : public VectorField
{
    SphereAttractor(const glm::vec3 &pos = glm::vec3(0.0f), float r = 10.0f)
        : mPosition(pos), mRadius(r)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const override
    {
        const glm::vec3 spherePoint = mPosition + glm::normalize(p - mPosition) * mRadius;
        return spherePoint - p;
    }
private:
    glm::vec3 mPosition;
    float mRadius;
};

struct SphereRepulsor : public VectorField
{
    SphereRepulsor(const glm::vec3 &pos = glm::vec3(0.0f), float r = 10.0f)
        : mPosition(pos), mRadius(r)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const override
    {
        const glm::vec3 spherePoint = mPosition + glm::normalize(p - mPosition) * mRadius;
        return p - spherePoint;
    }
private:
    glm::vec3 mPosition;
    float mRadius;
};

struct SphereRotator : public VectorField
{
    SphereRotator(const glm::vec3 &pos = glm::vec3(0.0f), float r = 10.0f)
        : mPosition(pos)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const override
    {
        static const glm::vec3 up(0.0f, 1.0f, 0.0f);
        return glm::cross(mPosition - p, up);
    }
private:
    glm::vec3 mPosition;
};

#endif
