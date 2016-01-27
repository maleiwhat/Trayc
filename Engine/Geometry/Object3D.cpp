/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Geometry/Object3D.h>

using namespace glm;

namespace engine
{
    Object3D::Object3D(TriangleMesh &triMesh, Material &material)
        : mIsActive(true), mIsMovable(false), mDynamicGeometry(false), mMaterial(material), mTriMesh(triMesh), mTransform(1.0f)
    {
    }

    void Object3D::SetTransform(const mat4 &transform)
    {
        mTransform = transform;
        CalcAABB();
    }

    const mat4& Object3D::GetTransform() const
    {
        return mTransform;
    }

    void Object3D::CalcAABB()
    {
        AABB meshAABB = mTriMesh.GetAABB();
        auto vertices = meshAABB.Vertices();

        for(int i = 0; i < vertices.size(); ++i)
            vertices[i] = vec3(mTransform * vec4(vertices[i], 1.0f));

        mAABB = AABB();
        mAABB |= vertices;
    }
}
