/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/GL/Program.h>
#include <Engine/Utils/ErrorCheck.h>
#include <easylogging++.h>
#include <fstream>

using namespace glm;
using namespace std;

namespace engine
{
    GLuint Program::mBoundProgram = 0;

    Program::Program(void)
        : mID(0)
    {
    }

    void Program::Destroy()
    {
        glDeleteProgram(mID);
        mID = 0;
    }

    void Program::Init(const VertexShader &vs, const GeometryShader &gs, const FragmentShader &fs, const char *name)
    {
        Destroy();
        mID = glCreateProgram();

        Attach(vs);
        Attach(gs);
        Attach(fs);

        LinkProgram(name);

        Detach(vs);
        Detach(gs);
        Detach(fs);

        CacheUniforms();
    }

    void Program::Init(const VertexShader &vs, const FragmentShader &fs, const char *name)
    {
        Destroy();
        mID = glCreateProgram();

        Attach(vs);
        Attach(fs);

        LinkProgram(name);

        Detach(vs);
        Detach(fs);

        CacheUniforms();
    }

    void Program::LinkProgram(const char *name)
    {
        glLinkProgram(mID);
        GLint status;
        glGetProgramiv(mID, GL_LINK_STATUS, &status);
        if(status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(mID, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetProgramInfoLog(mID, infoLogLength, nullptr, strInfoLog);

            LOG(ERROR) << "[Program::LinkProgram] Linking failure in program " << name << ":" << endl << strInfoLog;
            delete[] strInfoLog;
        }
    }

    void Program::CacheUniforms()
    {
        //Cache all the uniforms
        GLint uniformCount;
        glGetProgramiv(mID, GL_ACTIVE_UNIFORMS, &uniformCount);
        const GLsizei bufferSize = 100;
        for(GLint i = 0; i < uniformCount; ++i)
        {
            GLchar uniformName[bufferSize];
            GLsizei length;
            GLint size;
            GLenum type;
            glGetActiveUniform(mID, i, bufferSize, &length, &size, &type, uniformName);
            mUniformLocations[string(uniformName)] = glGetUniformLocation(mID, uniformName);
        }
    }

    void Program::Init(const char *name, const Shader::Defines &defines)
    {
        string sn(name);
        sn += ".geom";
        ifstream f(sn.c_str());

        if(f.good())
            Init(VertexShader(name, defines), GeometryShader(name, defines), FragmentShader(name, defines), name);
        else
            Init(VertexShader(name, defines), FragmentShader(name, defines), name);

        f.close();
    }

    void Program::Use() const
    {
        if(mID != mBoundProgram) {
            mBoundProgram = mID;
            glUseProgram(mID);
        }
    }

    void Program::Unbind()
    {
        glUseProgram(0);
        mBoundProgram = 0;
    }

    GLuint Program::GetUniformBlockLocation(const GLchar *name) const
    {
        return glGetUniformBlockIndex(mID, name);
    }

    void Program::SetUniformBlockBinding(const GLchar *name, GLuint bindingPoint) const
    {
        glUniformBlockBinding(mID, GetUniformBlockLocation(name), bindingPoint);
    }

    GLint Program::GetUniformi(const GLchar *name) const
    {
        GLint ret;
        glGetUniformiv(mID, mUniformLocations.find(name)->second, &ret);
        return ret;
    }

    void Program::SetUniform(const string &name, GLint x) const
    {
        glUniform1i(mUniformLocations.find(name)->second, x);
    }

    void Program::SetUniform(const string &name, GLfloat x) const
    {
        glUniform1f(mUniformLocations.find(name)->second, x);
    }

    void Program::SetUniform(const string &name, const vec2 &x) const
    {
        glUniform2f(mUniformLocations.find(name)->second, x.x, x.y);
    }

    void Program::SetUniform(const string &name, const vec3 &x) const
    {
        glUniform3f(mUniformLocations.find(name)->second, x.x, x.y, x.z);
    }

    void Program::SetUniform(const string &name, const vec4 &x) const
    {
        glUniform4f(mUniformLocations.find(name)->second, x.x, x.y, x.z, x.w);
    }

    void Program::SetUniform(const string &name, const mat3 &x) const
    {
        glUniformMatrix3fv(mUniformLocations.find(name)->second, 1, GL_FALSE, (float*)&x);
    }

    void Program::SetUniform(const string &name, const mat4 &x) const
    {
        glUniformMatrix4fv(mUniformLocations.find(name)->second, 1, GL_FALSE, (float*)&x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const GLfloat *x) const
    {
        glUniform1fv(mUniformLocations.find(name)->second, cnt, x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const vec2 *x) const
    {
        glUniform2fv(mUniformLocations.find(name)->second, cnt, (float*)x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const vec3 *x) const
    {
        glUniform3fv(mUniformLocations.find(name)->second, cnt, (float*)x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const vec4 *x) const
    {
        glUniform4fv(mUniformLocations.find(name)->second, cnt, (float*)x);
    }

    GLuint Program::GetID() const
    {
        return mID;
    }

    void Program::Attach(const Shader &sh) const
    {
        glAttachShader(mID, sh.mID);
    }

    void Program::Detach(const Shader &sh) const
    {
        glDetachShader(mID, sh.mID);
    }
}
