#include <OGL3D/Game/OGame.h>
#include <OGL3D/Minecraft/World.h>
#include <OGL3D/Window/OWindow.h>
#include <Windows.h>
#include <OGL3D/Minecraft/World.h>
#include <OGL3D/Time/OTime.h>
#include <OGL3D/InputSystem/OInputSystem.h>

OGame::OGame()
{
	
}

OGame::~OGame()
{
}

void OGame::run()
{
	World world;
	world.onCreate();

	MSG msg = {};
	while (m_isRunning)
	{

		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_isRunning = false;
				continue;
			}

			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		}
		OTime::get()->onUpdate();
		OInputSystem::get()->onUpdate();
		world.onUpdate();
	}

	world.onQuit();
}

void OGame::quit()
{
}
