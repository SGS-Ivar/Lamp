#include "Game.h"

#include <Lamp/Meshes/GeometrySystem.h>
#include <Lamp/Objects/Entity/BaseComponents/LightComponent.h>
#include <Lamp/Objects/Entity/BaseComponents/MeshComponent.h>

#include <Lamp/Objects/Entity/BaseComponents/CameraComponent.h>
#include <Lamp/Audio/AudioEngine.h>
#include <Lamp/Objects/Entity/BaseComponents/AudioListenerComponent.h>

void Game::OnStart()
{
	auto tempLevel = Lamp::LevelSystem::LoadLevel("assets/levels/Level.level");

	{
		Lamp::Brush* pBrush = Lamp::Brush::Create("assets/models/test1.lgf");
		pBrush->SetPosition({ 0.f, 5.f, 0.f });
		pBrush->GetPhysicalEntity()->GetPhysicsBody()->GetActor()->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
	}

	{
		Lamp::Brush* pBrush = Lamp::Brush::Create("assets/models/testGround.lgf");
		pBrush->SetRotation({ 90.f, 0.f, 0.f });
	}
}

bool Game::OnUpdate(Lamp::AppUpdateEvent& e)
{
	return false;
}

void Game::OnEvent(Lamp::Event& e)
{
	Lamp::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Lamp::AppUpdateEvent>(LP_BIND_EVENT_FN(Game::OnUpdate));
}