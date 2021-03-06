/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_FRAGMENTSHADER_H
#define EN_FRAGMENTSHADER_H

#include <Engine/GL/Shader.h>
#include <string>

namespace engine
{
    class FragmentShader :
        public Shader
    {
    public:
        FragmentShader(const char *name, const Defines &defines); // from file
        ~FragmentShader(void);

        const char* GetExtension() const override;
        const char* GetTypeString() const override;
        GLenum GetGlType() const override;
    };
}

#endif
