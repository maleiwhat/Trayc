/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/Renderer.h>
#include <Engine/Utils/StlExtensions.hpp>

using namespace glm;
using namespace std;
using namespace stdext;

namespace engine
{
    Renderer::Renderer(void)
        : mClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT), mCamera(nullptr)
    {
    }

    Renderer::~Renderer(void)
    {
        for(Program &p : mPrograms)
            p.Delete();
    }

    void Renderer::InitRendering(const Camera *camera)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        mCamera = camera;
        //TODO: Programs
    }

    void Renderer::Render() const
    {
        glClear(mClearMask);

        RenderingContext rContext;

        rContext.mV = mCamera->GetViewMatrix();
        rContext.mP = mCamera->GetProjectionMatrix();
        rContext.mVP = rContext.mP * rContext.mV;
        rContext.mCamera = mCamera;

        for(const Light &light : mLights)
        {
            if(light.mIsEnabled)
            {
                rContext.mLight = &light;
                rContext.mProgram = &mPrograms[light.mFlag];
                for(Renderable *renderable : mRenderables)
                {
                    if(renderable->mIsActive)
                    {
                        renderable->Draw(rContext);
                    }
                }
            }
        }
    }

    void Renderer::AddRenderable(Renderable *renderable)
    {
        mRenderables.push_back(renderable);
    }

    void Renderer::RemoveRenderable(Renderable *renderable)
    {
        erase(mRenderables, renderable);
    }

    const std::vector<Light>& Renderer::GetLights() const
    {
        return mLights;
    }

    const Light& Renderer::GetLight(int idx) const
    {
        return mLights[idx];
    }

    Light& Renderer::GetLight(int idx)
    {
        return mLights[idx];
    }

    void Renderer::AddLight(const Light &light)
    {
        mLights.push_back(light);
    }

    void Renderer::RemoveLight(int idx)
    {
        mLights.erase(mLights.begin() + idx);
    }

    void Renderer::SetClearColor(const glm::vec4 &clearColor) const
    {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    }
}