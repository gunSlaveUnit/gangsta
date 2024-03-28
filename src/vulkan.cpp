/*
find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})
*/

#include <vector>
#include <optional>
#include <iostream>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

using uf32 = uint_fast32_t;

struct QueueFamilyIndices {
    [[nodiscard]] bool available() const {
        return graphics_family.has_value();
    }

    std::optional<uf32> graphics_family;
};

bool check_validation_layers_available(const std::vector<const char *> &validation_layers) {
    uf32 validation_layers_amount;
    vkEnumerateInstanceLayerProperties(&validation_layers_amount, nullptr);

    std::vector<VkLayerProperties> available_validation_layers(validation_layers_amount);
    vkEnumerateInstanceLayerProperties(&validation_layers_amount, available_validation_layers.data());

    for (const char *layer_name: validation_layers) {
        bool is_layer_available = false;

        for (const auto &layerProperties: available_validation_layers)
            if (strcmp(layer_name, layerProperties.layerName) == 0) {
                is_layer_available = true;
                break;
            }

        if (!is_layer_available)
            return false;
    }

    return true;
}

std::vector<const char*> get_required_glfw_extensions() {
    uf32 glfw_extensions_amount = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extensions_amount);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extensions_amount);

    return extensions;
}

QueueFamilyIndices find_queue_families(const VkPhysicalDevice &physical_device) {
    QueueFamilyIndices indices{};

    uf32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
            physical_device,
            &queue_family_count,
            nullptr
    );

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(
            physical_device,
            &queue_family_count,
            queue_families.data()
    );

    uf32 graphics_family_index = 0;
    for (const auto &queue_family: queue_families) {
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphics_family = graphics_family_index;

        if (indices.available())
            break;

        ++graphics_family_index;
    }

    return indices;
}

bool is_physical_device_suitable(const VkPhysicalDevice &physical_device) {
    auto queue_families_indices = find_queue_families(physical_device);
    return queue_families_indices.available();
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    const uf32 WIDTH = 800;
    const uf32 HEIGHT = 600;
    const char *TITLE = "gangsta";

#ifdef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = false;
#else
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif

    const std::vector<const char *> VALIDATION_LAYERS{
            "VK_LAYER_KHRONOS_validation"
    };

    if (ENABLE_VALIDATION_LAYERS && !check_validation_layers_available((VALIDATION_LAYERS)))
        std::cerr << "ERROR: Vulkan validation layers requested, but not available\n";

    auto window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (window == nullptr)
        std::cerr << "ERROR: Failed to create a GLFW window\n";

    const char *APPLICATION_NAME = "gangsta";
    const auto APPLICATION_VERSION = VK_MAKE_VERSION(0, 0, 1);
    const char *ENGINE_NAME = "gangsta";
    const auto ENGINE_VERSION = VK_MAKE_VERSION(0, 0, 1);
    VkApplicationInfo application_info{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = APPLICATION_NAME,
            .applicationVersion = APPLICATION_VERSION,
            .pEngineName = ENGINE_NAME,
            .engineVersion = ENGINE_VERSION,
            .apiVersion = VK_API_VERSION_1_3
    };

    VkInstanceCreateInfo instance_create_info{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &application_info,
            .enabledLayerCount = ENABLE_VALIDATION_LAYERS ? static_cast<uf32>(VALIDATION_LAYERS.size()) : 0,
            .ppEnabledLayerNames = ENABLE_VALIDATION_LAYERS ? VALIDATION_LAYERS.data() : nullptr,
    };

    auto extensions = get_required_glfw_extensions();
    if (ENABLE_VALIDATION_LAYERS)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    instance_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instance_create_info.ppEnabledExtensionNames = extensions.data();

    VkInstance instance;
    if (vkCreateInstance(&instance_create_info, nullptr, &instance) != VK_SUCCESS)
        std::cerr << "ERROR: Failed to create a Vulkan instance\n";

    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
        std::cerr << "ERROR: Failed to create window surface\n";

    uf32 physical_devices_amount;
    vkEnumeratePhysicalDevices(instance, &physical_devices_amount, nullptr);

    std::vector<VkPhysicalDevice> physical_devices(physical_devices_amount);
    vkEnumeratePhysicalDevices(instance, &physical_devices_amount, physical_devices.data());

    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    for (const auto &device: physical_devices) {
        if (is_physical_device_suitable(device)) {
            physical_device = device;
            break;
        }
    }

    if (physical_device == VK_NULL_HANDLE)
        std::cerr << "ERROR: Failed to find a suitable physical device\n";

    auto queue_indices = find_queue_families(physical_device);
    float graphics_queue_priority = 1.0;
    VkDeviceQueueCreateInfo queue_create_info{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = queue_indices.graphics_family.value(),
            .queueCount = 1,
            .pQueuePriorities = &graphics_queue_priority
    };

    VkPhysicalDeviceFeatures physical_device_features{};

    VkDeviceCreateInfo logical_device_create_info{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queue_create_info,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = 0,
            .ppEnabledExtensionNames = nullptr,
            .pEnabledFeatures = &physical_device_features
    };

    VkDevice logical_device;
    if (vkCreateDevice(physical_device, &logical_device_create_info, nullptr, &logical_device) != VK_SUCCESS)
        std::cerr << "ERROR: Failed to create a Vulkan logical device\n";

    VkQueue graphics_queue;
    vkGetDeviceQueue(logical_device, queue_indices.graphics_family.value(), 0, &graphics_queue);

    while (!glfwWindowShouldClose(window))
        glfwPollEvents();

    vkDestroyDevice(logical_device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}