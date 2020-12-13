#include "AssetBrowser.h"

#include <imgui/imgui.h>
#include <vector>
#include <string>
#include <Lamp/Input/FileSystem.h>
#include <imgui/imgui_stdlib.h>

namespace Sandbox3D
{
	AssetBrowser::AssetBrowser()
	{
		m_Open = true;
	
		m_FileTexture = Lamp::Texture2D::Create("engine/textures/ui/TestFile.png");
		m_FolderTexture = Lamp::Texture2D::Create("engine/textures/ui/TestFolder.png");
	}

	void AssetBrowser::Update()
	{
		if (!m_Open)
		{
			return;
		}

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("Asset Browser", &m_Open);
		ImGui::PopStyleVar();

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceId = ImGui::GetID("assetbrowser");
			ImGui::DockSpace(dockspaceId, ImVec2(0.f, 0.f), dockspace_flags);
		}
		ImGui::End();

		UpdateFinder();
		UpdateViewer();
	}

	void AssetBrowser::OnEvent(Lamp::Event& e)
	{
		Lamp::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Lamp::AppFolderClickedEvent>(LP_BIND_EVENT_FN(AssetBrowser::OnFolderClickedEvent));
	}

	void AssetBrowser::UpdateFinder()
	{
		ImGui::Begin("Finder");
		{
			static std::string search = "";
			ImGui::InputText("Search", &search);

			if (ImGui::TreeNode("Assets"))
			{
				std::vector<std::string> folders = Lamp::FileSystem::GetAssetFolders();
				Lamp::FileSystem::PrintFolders(folders);

				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

	void AssetBrowser::UpdateViewer()
	{
		std::vector<std::string> foldersAndFiles;

		if (!m_SelectedFolderPath.empty())
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(m_SelectedFolderPath))
			{
				foldersAndFiles.push_back(entry.path().string());
			}
		}

		ImGui::Begin("Viewer");

		for (int i = 0; i < foldersAndFiles.size(); i++)
		{
			uint32_t texId = -1;
			std::string name = "";

			if (foldersAndFiles[i].find('.') == std::string::npos)
			{
				texId = m_FolderTexture->GetID();
				name = foldersAndFiles[i].substr(foldersAndFiles[i].find_last_of('\\') + 1, foldersAndFiles[i].size() - 1);
			}
			else
			{
				texId = m_FileTexture->GetID();
				name = foldersAndFiles[i].substr(foldersAndFiles[i].find_last_of('\\') + 1, foldersAndFiles[i].find_last_of('.'));
			}

			ImGui::Image((void*)(uint64_t)texId, ImVec2{ 64, 64 });

			//ImGui::SetCursorPos(cursorPos);
			ImGui::Text(name.c_str());

		}

		ImGui::End();
	}

	bool AssetBrowser::OnFolderClickedEvent(Lamp::AppFolderClickedEvent& e)
	{
		m_SelectedFolderPath = e.GetPath();

		return false;
	}
}