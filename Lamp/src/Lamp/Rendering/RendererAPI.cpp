#include "lppch.h"
#include "RendererAPI.h"

#include "Lamp/Core/Log.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/Direct3D11/Direct3D11RendererAPI.h"

namespace Lamp
{
#ifdef LP_PLATFORM_WINDOWS
#define	GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_VULKAN
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;
#else
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::DX11;
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#endif

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None: LP_CORE_ERROR("None is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLRendererAPI>();
			case RendererAPI::API::DX11: return CreateScope<Direct3D11RendererAPI>();
		}

		return nullptr;
	}

}