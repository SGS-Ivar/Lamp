#include <Lamp.h>

#include "Lamp/Core/EntryPoint.h"

#include "GameLauncher/GameLauncher.h"
#define LP_CLIENT

class GameApp : public Lamp::Application
{
public:
	GameApp()
	{
		PushLayer(new GameLauncher::GameLauncher());
	};
	~GameApp() {};
};

Lamp::Application* Lamp::CreateApplication()
{
	return new GameApp();
}