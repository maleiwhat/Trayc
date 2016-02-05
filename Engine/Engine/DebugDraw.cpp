
#include <Engine/Engine/DebugDraw.h>
#include <Engine/Engine/AssetLoader.h>

#include <Engine/Engine/GlobalRenderingParams.h>

using namespace glm;
using namespace std;

namespace engine
{
    void DebugDraw::DrawDepth(const Texture2D &tex, float nearRange, float farRange) const
    {
        TextureCombiner::SetTexture(TextureType::DEBUG_SLOT, tex);
        mDrawDepth.Prog().Use();
        mDrawDepth.Prog().SetUniform("nearRange", nearRange);
        mDrawDepth.Prog().SetUniform("farRange", farRange);
        mDrawDepth.Draw();
        Program::Unbind();
    }

    void DebugDraw::DrawTexture(const Texture2D &tex) const
    {
        TextureCombiner::SetTexture(TextureType::DEBUG_SLOT, tex);
        mDrawTex.Prog().Use();
        mDrawTex.Draw();
        Program::Unbind();
    }

    void DebugDraw::DrawNormal(const Texture2D &tex) const
    {
        TextureCombiner::SetTexture(TextureType::DEBUG_SLOT, tex);
        mDrawNormals.Prog().Use();
        mDrawNormals.Draw();
        Program::Unbind();
    }

    void DebugDraw::DrawGloss(const Texture2D &tex) const
    {
        TextureCombiner::SetTexture(TextureType::DEBUG_SLOT, tex);
        mDrawGloss.Prog().Use();
        mDrawGloss.Draw();
        Program::Unbind();
    }

    void DebugDraw::Destroy()
    {
        mDrawDepth.Destroy();
        mDrawTex.Destroy();
        mDrawNormals.Destroy();
        mDrawGloss.Destroy();
    }

    DebugDraw::DebugDraw(void)
    {
        const string vsPath = AssetLoader::ShaderPath("C_TexToScreen").data();

        mDrawDepth.Init(vsPath.data(), AssetLoader::ShaderPath("C_DepthToScreen").data());
        mDrawDepth.Prog().Use();
        mDrawDepth.Prog().SetUniform("tex", TextureType::DEBUG_SLOT);
        
        mDrawTex.Init(vsPath.data(), AssetLoader::ShaderPath("C_TexToScreen").data());
        mDrawTex.Prog().Use();
        mDrawTex.Prog().SetUniform("tex", TextureType::DEBUG_SLOT);

        mDrawNormals.Init(vsPath.data(), AssetLoader::ShaderPath("C_NormToScreen").data());
        mDrawNormals.Prog().Use();
        mDrawNormals.Prog().SetUniform("tex", TextureType::DEBUG_SLOT);

        mDrawGloss.Init(vsPath.data(), AssetLoader::ShaderPath("C_GlossToScreen").data());
        mDrawGloss.Prog().Use();
        mDrawGloss.Prog().SetUniform("tex", TextureType::DEBUG_SLOT);

        Program::Unbind();
    }
}
