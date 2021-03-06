#ifndef EN_VIEW_RAY_DATA_UB_H
#define EN_VIEW_RAY_DATA_UB_H

#include <Engine/GL/UniformBuffer.h>
#include <glm/glm.hpp>

namespace engine
{
    class ViewRayDataUB : public UniformBuffer
    {
    public:
        ViewRayDataUB(void);

        UNIFORM_MACRO(float, tanHalfFovy, 0); // tan(fovy / 2)
        UNIFORM_MACRO(float, aspectTanHalfFovy, 4); // aspectRatio * tan(fovy / 2)
    };

    class MatricesUB : public UniformBuffer
    {
    public:
        MatricesUB(void);

        UNIFORM_MACRO(glm::mat4, V, 0 * sizeof(glm::mat4));
        UNIFORM_MACRO(glm::mat4, P, 1 * sizeof(glm::mat4));
        UNIFORM_MACRO(glm::mat4, VP, 2 * sizeof(glm::mat4));
        UNIFORM_MACRO(glm::mat4, invV, 3 * sizeof(glm::mat4));
        UNIFORM_MACRO(glm::mat4, invP, 4 * sizeof(glm::mat4));
        UNIFORM_MACRO(glm::mat4, invVP, 5 * sizeof(glm::mat4));
    };
}

#endif
