#include "lppch.h"
#include "VulkanContext.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <set>

namespace Lamp
{
	static VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			LP_CORE_INFO(pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			LP_CORE_INFO(pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			LP_CORE_WARN(pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			LP_CORE_ERROR(pCallbackData->pMessage);
			break;
		}
	}

	VulkanContext::VulkanContext(GLFWwindow* windowHandle)
		: m_pWindowHandle(windowHandle)
	{
#ifdef LP_ENABLE_ASSERTS
		LP_CORE_ASSERT(windowHandle, "Window handle is null!");
#endif
	}

	VulkanContext::~VulkanContext()
	{
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkDestroyDevice(m_Device, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}

	void VulkanContext::Initialize()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		LP_CORE_INFO("Maximum {0} extensions supported", extensionCount);

		VkApplicationInfo aI = {};
		aI.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		aI.pApplicationName = "Lamp";
		aI.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		aI.pEngineName = "None";
		aI.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		aI.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo cI = {};
		cI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		cI.pApplicationInfo = &aI;

		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

		auto extensions = GetRequiredExtensions();

		cI.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		cI.ppEnabledExtensionNames = extensions.data();
		cI.enabledLayerCount = 0;

#ifdef LP_DEBUG

		cI.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
		cI.ppEnabledLayerNames = m_ValidationLayers.data();

		if (!CheckValidationLayerSupport())
		{
			LP_CORE_CRITICAL("Validation layers requested but not found!");
		}
#endif

		LP_CORE_ASSERT(vkCreateInstance(&cI, nullptr, &m_Instance) == VK_SUCCESS, "Failed to create instance!");

		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();
	}

	void VulkanContext::SwapBuffers()
	{
	}

	bool VulkanContext::CheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availibleLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availibleLayers.data());

		for (const char* layerName : m_ValidationLayers)
		{
			bool layerFound = false;
			for (const auto& layerProperties : availibleLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> VulkanContext::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;

		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef LP_DEBUG
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

		return extensions;
	}

	void VulkanContext::PickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			LP_CORE_ERROR("No Vulkan supported devices found!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());
	
		for (const auto& device : devices)
		{
			if (IsDeviceSuitable(device))
			{
				m_PhysicalDevice = device;
				break;
			}
		}

		if (m_PhysicalDevice == VK_NULL_HANDLE)
		{
			LP_CORE_CRITICAL("Failed to find suitable device!");
		}
	}

	bool VulkanContext::IsDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = FindQueueFamilies(device);
		return indices.IsComplete();
	}

	void VulkanContext::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

		std::vector<VkDeviceQueueCreateInfo> qcis;
		std::set<uint32_t> uqf = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

		float qPriority = 1.f;
		for (uint32_t qF : uqf)
		{
			VkDeviceQueueCreateInfo qci{};
			qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			qci.queueFamilyIndex = qF;
			qci.queueCount = 1;
			qci.pQueuePriorities = &qPriority;
			qcis.push_back(qci);
		}


		VkPhysicalDeviceFeatures df = {};

		VkDeviceCreateInfo ci = {};
		ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		ci.pQueueCreateInfos = qcis.data();
		ci.queueCreateInfoCount = static_cast<uint32_t>(qcis.size());
		ci.pEnabledFeatures = &df;
		ci.enabledExtensionCount = 0;
		ci.enabledLayerCount = 0;

#ifdef LP_DEBUG
		ci.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
		ci.ppEnabledLayerNames = m_ValidationLayers.data();
#endif

		LP_CORE_ASSERT(vkCreateDevice(m_PhysicalDevice, &ci, nullptr, &m_Device) == VK_SUCCESS, "Failed to create logical device!");

		vkGetDeviceQueue(m_Device, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, indices.PresentFamily.value(), 0, &m_PresentQueue);

		VkWin32SurfaceCreateInfoKHR cik = {};
		cik.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		cik.hwnd = glfwGetWin32Window(m_pWindowHandle);
		cik.hinstance = GetModuleHandle(nullptr);

		LP_CORE_ASSERT(vkCreateWin32SurfaceKHR(m_Instance, &cik, nullptr, &m_Surface) == VK_SUCCESS, "Failed to create window surface!");
	}

	void VulkanContext::CreateSurface()
	{
		LP_CORE_ASSERT(glfwCreateWindowSurface(m_Instance, m_pWindowHandle, nullptr, &m_Surface) == VK_SUCCESS, "Failed to create window surface!");
	}

	QueueFamilyIndices VulkanContext::FindQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.GraphicsFamily = i;
			}

			if (indices.IsComplete())
			{
				break;
			}

			VkBool32 pS = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &pS);
			if (pS)
			{
				indices.PresentFamily = i;
			}

			i++;
		}

		return indices;
	}
}