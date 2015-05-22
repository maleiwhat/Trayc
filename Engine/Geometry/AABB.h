/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_AABB_H
#define EN_AABB_H

#include <glm/glm.hpp>

namespace engine
{
    struct AABB
    {
        AABB(void);

        AABB& operator|=(const glm::vec3 &p);
        AABB& operator|=(const AABB &box);

        glm::vec3 minv;
        glm::vec3 maxv;
    };

    AABB operator|(const AABB &box, const glm::vec3 &p);
    AABB operator|(const AABB &box1, const AABB &box2);
}

#endif