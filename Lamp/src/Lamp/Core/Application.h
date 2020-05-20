#pragma once
#include "Window.h"
#include "Lamp/Rendering/Renderer.h"
#include "Lamp/Event/Event.h"

#include "Lamp/Event/ApplicationEvent.h"
#include "Lamp/Layer/Layer.h"
#include "Lamp/Layer/LayerStack.h"

#include "Lamp/ImGui/ImGuiLayer.h"
#include "Lamp/Input/FileSystem.h"
#include "Time/FrameTime.h"

namespace Lamp
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* pLayer);
		void PushOverlay(Layer* pLayer);

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void OnItemClicked(File& file);

		inline static Application& Get() { return *s_pInstance; }
		inline Window& GetWindow() { return *m_pWindow; }
		inline const FrameTime& GetFrameTime() { return m_FrameTime; }

	private:
		static Application* s_pInstance;

	private:
		std::unique_ptr<Window> m_pWindow;
		ImGuiLayer* m_pImGuiLayer;
		LayerStack m_LayerStack;

		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.f;

		FrameTime m_FrameTime;
	};
	
	static Application* CreateApplication();
}