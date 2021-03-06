/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "BallRenderer.h"
#include <vector>
#include <Engine/Utils/MathFunctions.h>
#include <random>

using namespace engine;
using namespace glm;
using namespace std;

BallRenderer::BallRenderer(const RenderingParams &rParams, int ctBalls, float cubeSize, float ballRadius)
    : mCtBalls(ctBalls)
{
    mProgram.Init("Shaders/PointSpheres");

    glGenBuffers(1, &mVBO);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);
    vector<vec3> colors;
    colors.reserve(mCtBalls);

    for(int i = 0; i < mCtBalls; ++i)
        colors.push_back(normalize(vec3(dis(gen) * 2.0f, dis(gen) * 1.1f, dis(gen) * 1.5f)));

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        //Allocate space for positions and colors
        glBufferData(GL_ARRAY_BUFFER, mCtBalls * sizeof(vec3) * 2, nullptr, GL_STREAM_DRAW);
        //Copy color buffer
        glBufferSubData(GL_ARRAY_BUFFER, mCtBalls * sizeof(vec3), mCtBalls * sizeof(vec3), colors.data());
        //Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        //Color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(mCtBalls * sizeof(vec3)));
    }
    glBindVertexArray(0);

    mProgram.Use();
    mProgram.SetUniform("lightDir", rParams.mLight.mPosition);
    mProgram.SetUniform("pointScale", float(rParams.mSSize.y) / tanf(rParams.mFOV * HALF_PI / 180.0f));
    mProgram.SetUniform("pointRadius", ballRadius);
    mProgram.Unbind();

    glEnable(GL_PROGRAM_POINT_SIZE);
}

BallRenderer::~BallRenderer(void)
{
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
    mProgram.Delete();
}

void BallRenderer::SetBalls(const vec3 *balls) const
{
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mCtBalls * sizeof(vec3), balls);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BallRenderer::Draw(const RenderingContext &rContext) const
{
    mProgram.Use();

    mProgram.SetUniform("MVP", rContext.mVP);
    mProgram.SetUniform("V", rContext.mV);
    mProgram.SetUniform("P", rContext.mP);
    mProgram.SetUniform("cameraPos", rContext.mCamera->mPosition);

    glBindVertexArray(mVAO);
    glDrawArrays(GL_POINTS, 0, mCtBalls);
    glBindVertexArray(0);
    mProgram.Unbind();
    glDepthFunc(GL_LEQUAL);
}
