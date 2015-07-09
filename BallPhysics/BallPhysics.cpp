/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "BallPhysics.h"
#include <assert.h>
#include <vector>
#include <random>
#include <iostream>

using namespace engine;
using namespace glm;
using namespace std;

BallPhysics::BallPhysics(PhysicsSolver *solver, int ctBalls, float cubeSize, float ballRadius)
    : mSolver(solver), mCtBalls(ctBalls), mCubeSize(cubeSize), mBallRadius(ballRadius)
{
}

void BallPhysics::Update(float dt)
{
    mSolver->Solve(dt);
}

std::vector<glm::vec3> BallPhysics::GetBallPositions() const
{
    const auto &balls = mSolver->GetBalls();
    vector<glm::vec3> positions(mCtBalls);
    for(int i = 0; i < mCtBalls; ++i)
        positions[i] = balls[i].mPosition;
    return positions;
}

void BallPhysics::InitBalls()
{
    const int ballsPerRow = int(mCubeSize / mBallRadius) - 1;

    if(ballsPerRow * ballsPerRow * ballsPerRow < mCtBalls)
    {
        cerr << "Balls dont fint into the box." << endl;
        std::exit(-1);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    const std::uniform_real_distribution<float> dis(-mBallRadius * 0.05f, mBallRadius * 0.05f);

    std::vector<Ball> balls;
    int ctBalls = 0;
    for(int i = 0; i < ballsPerRow; ++i)
    {
        const float x = float(i * 2 + 2) * mBallRadius - mCubeSize;
        for(int j = 0; j < ballsPerRow; ++j)
        {
            const float z = float(j * 2 + 2) * mBallRadius - mCubeSize;
            for(int k = 0; k < ballsPerRow; ++k)
            {
                if(ctBalls == mCtBalls)
                    break;

                const float y = float(k * 2 + 2) * mBallRadius - mCubeSize;
                balls.push_back(Ball(vec3(x + dis(gen), y + dis(gen), z + dis(gen))));
                ++ctBalls;
            }
            if(ctBalls == mCtBalls)
                break;
        }
        if(ctBalls == mCtBalls)
            break;
    }

    mSolver->SetBalls(balls);
}