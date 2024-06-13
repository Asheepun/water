#include "engine/gvk.h"

bool GVK_PRINT_STUFF = false;

VkInstance gvk_instance;
VkDevice gvk_device;
VkPhysicalDevice *gvk_physicalDevice_p;
VkQueue gvk_queue;

int32_t gvk_queueFamilyIndex;

void gvk_init(){

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "vulkan test";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName = "vulkan engine";
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	uint32_t n_layerProperties;
	vkEnumerateInstanceLayerProperties(&n_layerProperties, NULL);

	VkLayerProperties layerProperties[n_layerProperties];
	vkEnumerateInstanceLayerProperties(&n_layerProperties, layerProperties);

	const char *validationLayerNames[] = {
		"VK_LAYER_KHRONOS_validation",
	};

	int n_validationLayerNames = sizeof(validationLayerNames) / sizeof(const char *);

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.ppEnabledLayerNames = validationLayerNames;
	instanceCreateInfo.enabledLayerCount = n_validationLayerNames;

	VkResult result = vkCreateInstance(&instanceCreateInfo, NULL, &gvk_instance);
	if(result != 0) printf("ERROR\nvkCreateInstance result: %i\n", result);

	uint32_t n_physicalDevices = 0;
	vkEnumeratePhysicalDevices(gvk_instance, &n_physicalDevices, NULL);

	VkPhysicalDevice physicalDevices[n_physicalDevices];
	vkEnumeratePhysicalDevices(gvk_instance, &n_physicalDevices, physicalDevices);

	if(GVK_PRINT_STUFF) printf("n_physicalDevices: %i\n", n_physicalDevices);

	VkPhysicalDeviceProperties physicalDeviceProperties[n_physicalDevices];
	for(int i = 0; i < n_physicalDevices; i++){
		vkGetPhysicalDeviceProperties(physicalDevices[i], &physicalDeviceProperties[i]);
		print(physicalDeviceProperties[i].deviceName);
	}

	gvk_physicalDevice_p = &physicalDevices[1];

	uint32_t n_queueFamilyProperties = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(*gvk_physicalDevice_p, &n_queueFamilyProperties, NULL);

	VkQueueFamilyProperties queueFamilyProperties[n_queueFamilyProperties];
	vkGetPhysicalDeviceQueueFamilyProperties(*gvk_physicalDevice_p, &n_queueFamilyProperties, queueFamilyProperties);

	gvk_queueFamilyIndex = -1;

	for(int i = 0; i < n_queueFamilyProperties; i++){
		if(queueFamilyProperties[i].queueCount > 0
		&& (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)){
			gvk_queueFamilyIndex = i;
			break;
		}
	}

	if(GVK_PRINT_STUFF) printf("n_queueFamilyProperties: %i\n", n_queueFamilyProperties);
	if(GVK_PRINT_STUFF) printf("queueFamilyIndex: %i\n", gvk_queueFamilyIndex);

	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.queueFamilyIndex = gvk_queueFamilyIndex;
	deviceQueueCreateInfo.queueCount = 1;
	deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.ppEnabledLayerNames = validationLayerNames;
	deviceCreateInfo.enabledLayerCount = n_validationLayerNames;

	result = vkCreateDevice(*gvk_physicalDevice_p, &deviceCreateInfo, NULL, &gvk_device);
	if(result != 0) printf("ERROR\nvkCreateDevice result: %i\n", result);

	vkGetDeviceQueue(gvk_device, gvk_queueFamilyIndex, 0, &gvk_queue);

}

void Buffer_init(Buffer *buffer_p, int size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags memoryPropertyFlags){

	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usage;
	bufferCreateInfo.sharingMode = sharingMode;
	
	VkResult result = vkCreateBuffer(gvk_device, &bufferCreateInfo, NULL, &buffer_p->ID);
	if(result != 0) printf("ERROR\nvkCreateBuffer result: %i\n", result);

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(gvk_device, buffer_p->ID, &memoryRequirements);

	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(*gvk_physicalDevice_p, &memoryProperties);

	int memoryTypeIndex = -1;

	for(int i = 0; i < memoryProperties.memoryTypeCount; i++){
		if((memoryRequirements.memoryTypeBits & (1 << i))
		&& (memoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags){
			memoryTypeIndex = i;
			break;
		}
	}

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

	result = vkAllocateMemory(gvk_device, &memoryAllocateInfo, NULL, &buffer_p->memory);
	if(result != 0) printf("ERROR\nvkAllocateMemory result: %i\n", result);

	result = vkBindBufferMemory(gvk_device, buffer_p->ID, buffer_p->memory, 0);
	if(result != 0) printf("ERROR\nvkBindBufferMemory result: %i\n", result);

}
