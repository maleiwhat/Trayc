/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_TRIANGLEMESH_H
#define EN_TRIANGLEMESH_H

#include <Engine/Geometry/AABB.h>
#include <Engine/Geometry/Sphere.h>
#include <Engine/GL/VertexAttributeDef.h>
#include <glm/glm.hpp>
#include <vector>

namespace engine
{
    class TriangleMesh
    {
    public:
        TriangleMesh(unsigned int drawMode);

        std::vector<glm::vec3> mPositions;
        std::vector<glm::vec2> mUVs;
        std::vector<glm::vec3> mNormals;
        std::vector<glm::vec3> mTangents;
        std::vector<glm::vec3> mBitangents;
        std::vector<unsigned int> mIndices;

        void Clear();
        void FlipNormals();
        void FlipWinding();

        std::vector<VertexAttribDef> GetVertexAttribDefs() const;
        GLenum GetIndexType() const;
        void* GetIndices() const;
        std::vector<float> GetVertexArray() const;

        // In number of elements
        int GetVertexSize() const;
        int GetVertexSizeInBytes() const;

        const AABB& GetAABB() const;
        const Sphere& GetBSphere() const;
        void CalcBoundingVolumes();

        unsigned int GetDrawMode() const;

    private:
        friend class AssetLoader;
        void SetBoundingVolumes(const AABB &box, const Sphere &bSphere);

        unsigned int mDrawMode;
        AABB mAABB;
        Sphere mBSphere;
    };
}

#endif
