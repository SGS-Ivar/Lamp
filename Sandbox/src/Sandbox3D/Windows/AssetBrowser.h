#pragma once

#include <Lamp/Event/Event.h>
#include <Lamp/Event/ApplicationEvent.h>
#include <Lamp/Rendering/Texture2D/Texture2D.h>

namespace Sandbox3D
{
	class AssetBrowser
	{
	public:
		AssetBrowser(uint32_t id);
		void Update();

		inline bool& GetIsOpen() { return m_Open; }

		void OnEvent(Lamp::Event& e);

	private:
		void UpdateFinder();
		void UpdateViewer();

		bool OnFolderClickedEvent(Lamp::AppFolderClickedEvent& e);

	private:
		bool m_Open = false;
		Ref<Lamp::Texture2D> m_FolderTexture;
		Ref<Lamp::Texture2D> m_FileTexture;
		
		std::string m_SelectedFolderPath = "";
		uint32_t m_Id;

		//Settings
		glm::vec2 m_IconSize = { 64.f, 64.f };

	};
}