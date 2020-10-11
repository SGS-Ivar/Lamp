#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include "Lamp/Rendering/GraphicsContext.h"
#include <vulkan/vulkan.h>
#include <optional>

struct GLFWwindow;

namespace Lamp
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;
		std::optional<uint32_t> PresentFamily;
		bool IsComplete()
		{
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}
	};

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);
		~VulkanContext();

		virtual void Initialize() override;
		virtual void SwapBuffers() override;

	private:
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();

		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSurface();

		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	private:
		const std::vector<const char*> m_ValidationLayers =
		{
			"VK_LAYER_KHRONOS_validation"
		};

		GLFWwindow* m_pWindowHandle;
		VkInstance m_Instance;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device;
		VkQueue m_GraphicsQueue;
		VkSurfaceKHR m_Surface;
		VkQueue m_PresentQueue;
	};
}