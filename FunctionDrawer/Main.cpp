/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/DefaultCameraHandler.h>

#include "UserSettings.h"
#include "GUIHandler.h"

using namespace engine;
using namespace std;
using namespace glm;

void RenderingLoop(char const *programName)
{
    DefaultCameraHandler camHandler(Camera(vec3(7.0f, 9.2f, -6.0f), (float)UserSettings::Get().screenWidth / UserSettings::Get().screenHeight,
        UserSettings::Get().FOV, 0.1f, UserSettings::Get().drawDistance), 7.0f, 0.006f);

    Scene scene(1.0f / 60.0f);

    scene.Init(&camHandler, programName, UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);

    //Init AntTweakBar
    TwInit(TW_OPENGL_CORE, nullptr);

    TwWindowSize(UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);

    scene.mRenderer.SetClearColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));

    GUIHandler guiHandler;
    guiHandler.CreateTweakBars(&scene);
    scene.mInputHandler.AddEventListener(&guiHandler);
    scene.mRenderer.AddRenderable(&guiHandler);

    scene.mRenderer.AddLight(Light());
    scene.GameLoop();

    TwTerminate();
}

int main(int argc, char *argv[])
{
    RenderingLoop(argv[0]);
    return 0;
}
