/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_GAMEENGINE_H
#define TRAYC_GAMEENGINE_H

#include <Engine/Core/Updateable.h>
#include <Engine/Core/EventListener.h>
#include <Trayc/Player.h>
#include <Trayc/OptixTracer.h>
#include <Trayc/BufferDrawer.h>

namespace trayc
{
	class GameEngine :
		public engine::Updateable, public engine::EventListener
	{
	public:
		GameEngine(void);
		~GameEngine(void);

        void HandleEvent(const SDL_Event &e);

        /*void mouseClick(const reng::MouseClickEvent &e);*/

		void Update(float deltaTime);

		void Init();
		void Draw();

		Player *player;
		OptixTracer tracer;
	private:
		BufferDrawer drawer;
		bool mouseLocked;
	};
}

#endif
