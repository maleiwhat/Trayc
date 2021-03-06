/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_OPTIXTEXTUREHANDLER_H
#define TRAYC_OPTIXTEXTUREHANDLER_H

#include <GL/glew.h>
#include <optix_world.h>
#include <map>
#include <Trayc/Utils.h>
#include <Engine/Utils/Singleton.h>

namespace trayc
{
    class OptixTextureHandler : public engine::Singleton<OptixTextureHandler>
    {
    public:
        using engine::Singleton<OptixTextureHandler>::Get;

        optix::TextureSampler Get(const std::string &path, GLfloat anisotropy = 0.0f, RTwrapmode wrapMode = RT_WRAP_REPEAT, 
            const std::string &def = Utils::DefTexture("error.png"), GLenum format = GL_RGBA8);

        void CleanUP();

    private:
        OptixTextureHandler(void);
        friend engine::Singleton<OptixTextureHandler>;
        std::map<std::string, std::pair<optix::TextureSampler, GLuint> > existingTextures;
    };
}

#endif
