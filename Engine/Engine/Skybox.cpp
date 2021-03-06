/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/Skybox.h>
#include <Engine/Geometry/GeometryMeshes.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Core/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

namespace engine
{
    Skybox::Skybox(void)
        : mSkyboxVA(GL_STATIC_DRAW), mFarPlaneMod(30.0f)
    {
    }

    void Skybox::Init(const Renderer *renderer)
    {
        mSkyboxProg.Init(AssetLoader::Get().ShaderPath("F_Skybox").data());
        mSkyboxProg.Use();
        mSkyboxProg.SetUniform("skybox", TextureType::SKYBOX_SLOT);
        Program::Unbind();

        // Load skybox
        const string sideNames[6] = {
            AssetLoader::Get().TexturePath("skybox/front.jpg"),
            AssetLoader::Get().TexturePath("skybox/back.jpg"),
            AssetLoader::Get().TexturePath("skybox/top.jpg"),
            AssetLoader::Get().TexturePath("skybox/bottom.jpg"),
            AssetLoader::Get().TexturePath("skybox/left.jpg"),
            AssetLoader::Get().TexturePath("skybox/right.jpg")
        };
        mSkyboxCubemap.Init(sideNames);
        mSkyboxCubemap.BindToSlot(TextureType::SKYBOX_SLOT);

        const float sideHalfSize = renderer->GetCamera()->mFarDistance * mFarPlaneMod * 0.5f;
        mSkyboxTransform = scale(mat4(1.0f), vec3(sideHalfSize));
        mSkyboxTransform = rotate(mSkyboxTransform, radians(180.0f), vec3(0.0f, 0.0f, 1.0f));
        TriangleMesh cube = GetCubeMeshSolid(true, false);
        cube.FlipWinding();
        mSkyboxVA.Init(&cube);
    }

    void Skybox::Destroy()
    {
        mSkyboxProg.Destroy();
        mSkyboxCubemap.Destroy();
        mSkyboxVA.Destroy();
    }

    void Skybox::Draw(const Camera *camera) const
    {
        Camera skyBoxCam(*camera);
        skyBoxCam.mFarDistance *= mFarPlaneMod;
        const mat4 VP = skyBoxCam.GetProjectionMatrix() * skyBoxCam.GetViewMatrix();

        glDepthRange(1.0, 1.0);
        mSkyboxProg.Use();
        mSkyboxProg.SetUniform("MVP", VP * mSkyboxTransform);
        mSkyboxVA.Render(GL_TRIANGLES);
        glDepthRange(0.0, 1.0);
    }
}
