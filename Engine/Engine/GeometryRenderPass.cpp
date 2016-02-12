/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Engine/AssetLoader.h>

#include <easylogging++.h>

using namespace glm;
using namespace std;
using namespace stdext;

namespace engine
{
    GeometryRenderPass::GeometryRenderPass(void)
        : RenderPass("gPass")
    {
    }

    void GeometryRenderPass::Init()
    {
        int width = 1280;
        int height = 720;

        // Init G buffer
        mDstFB.Init(width, height);
        mDstFB.AddAttachment(GL_R32F, GL_RED, GL_FLOAT); //Linear Depth
        mDstFB.AddAttachment(GL_RGBA16F, GL_RGBA, GL_FLOAT); //Normal view space / x
        mDstFB.AddAttachment(GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE); //Specular / Gloss
        mDstFB.AddAttachment(GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE); //Albedo / x
        mDstFB.AttachRBO(); // For depth testing
        mDstFB.Compile();

        // Init material to program map, and material to vertex arrays map
        const int ctProgs = 1 << TextureType::CT_MAT_TEXTURE_TYPES;
        for(int i = 0; i < ctProgs; ++i) {
            int progDefines = 0;
            vector<string> defines;

            for(int j = 0; j < TextureType::CT_MAT_TEXTURE_TYPES; ++j) {
                if((1 << j) & i) {
                    progDefines |= (1 << j);
                    defines.push_back(MAT_TEXTURE_DEFINES[j]);
                }
            }

            // Init program with height map only if it has a normal map
            const int hasNormal = progDefines & (1 << TextureType::NORMAL_MAP);
            const int hasHeight = progDefines & (1 << TextureType::HEIGHT_MAP);
            if(!hasHeight || hasNormal) {
                mGPrograms[progDefines].Init(AssetLoader::Get().ShaderPath("G_GeometryPass").data(), defines);
            }
        }

        // Init Texture Sampling
        mTexMapSampler.InitForDiffuse();
        mTexMapSampler.BindToSlot(0);
        mTexMapSampler.BindToSlot(1);
        mTexMapSampler.BindToSlot(2);
        mTexMapSampler.BindToSlot(3);
    }

    void GeometryRenderPass::Destroy()
    {
        mDstFB.Destroy();
        ClearVertexArrays();
        for(Program &p : mGPrograms)
            p.Delete();

        for(auto &pst : mNameToTex)
            pst.second.Delete();

        mTexMapSampler.Destroy();
    }

    void GeometryRenderPass::BeginRender() const
    {
        mDstFB.Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GeometryRenderPass::EndRender() const
    {
        FrameBuffer::UnBind();
    }

    void GeometryRenderPass::Render(const vector<RenderPass*> &renderPasses, const RenderingContext &rContext) const
    {
        for(const Object3D &obj : mScene->mObjects3D)
        {
            const VertexArray &VA = mVertexArrays[obj.GetMeshIdx()];
            const Material &mat = mScene->mMaterials[obj.GetMaterialIdx()];
            const int renderFlags = mat.GetRenderFlags();
            const Program &prog = mGPrograms[renderFlags];

            prog.Use();

            // Mesh uniforms
            prog.SetUniform("MVP", rContext.mVP * obj.GetTransform());
            prog.SetUniform("MV", rContext.mV * obj.GetTransform());

            // Material uniforms
            prog.SetUniform("diffuseColor", mat.mKd);
            prog.SetUniform("specularGloss", vec4(mat.mKs, mat.mGloss));

            // Textures
            for(const Material::TextureInfo &texInfo : mat.mTextureMaps) {
                prog.SetUniform(TEXTURE_UNIFORM_NAMES[texInfo.type], texInfo.type);
                glActiveTexture(GL_TEXTURE0 + texInfo.type);
                mNameToTex.at(texInfo.name).Bind();
            }

            VA.RenderIndexed(GL_TRIANGLES);
        }
    }

    void GeometryRenderPass::InitScene(const Scene *scene)
    {
        mScene = scene;

        // Load mesh to memory
        for(const TriangleMesh &mesh : scene->mTriMeshes)
        {
            mVertexArrays.push_back(VertexArray(GL_STATIC_DRAW));
            VertexArray &VA = mVertexArrays.back();

            const int ctVertices = mesh.mPositions.size();
            const int ctIndices = mesh.mIndices.size();

            VA.AddAttributes(mesh.GetVertexAttribDefs());
            VA.Init(ctVertices, ctVertices);

            VA.SetVertices(static_cast<const GLvoid*>(mesh.GetVertexArray().data()), 0, ctVertices);

            if(ctIndices > 0)
            {
                void *indices = mesh.GetIndices();
                VA.SetIndices(indices, ctIndices, mesh.GetIndexType());
                delete[] indices;
            }
        }
        LOG(INFO) << "Loaded meshes to GPU.";
        // Load textures to memory
        for(const Material &mat : scene->mMaterials) {
            for(const Material::TextureInfo &texInfo : mat.mTextureMaps) {
                if(mNameToTex.find(texInfo.name) == mNameToTex.end()) {
                    mNameToTex[texInfo.name] = Texture2D(texInfo.name.c_str());
                }
            }
        }
        LOG(INFO) << "Loaded textures to GPU.";
    }

    void GeometryRenderPass::ClearVertexArrays()
    {
        for(auto &VA : mVertexArrays)
            VA.Destroy();

        mVertexArrays.clear();
    }
}
