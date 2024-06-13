#include "stdio.h"
#include "math.h"

#include "gvk.cpp"

uint32_t* readFile(uint32_t& length, const char* filename) {

	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("Could not find or open file: %s\n", filename);
	}

	// get file size.
	fseek(fp, 0, SEEK_END);
	long filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	long filesizepadded = long(ceil(filesize / 4.0)) * 4;

	// read file contents.
	char *str = new char[filesizepadded];
	fread(str, filesize, sizeof(char), fp);
	fclose(fp);

	// data padding. 
	for (int i = filesize; i < filesizepadded; i++) {
		str[i] = 0;
	}

	length = filesizepadded;
	return (uint32_t *)str;
}

int main(){

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

	printf("vkCreateInstance result: %i\n", result);

	uint32_t n_physicalDevices = 0;
	vkEnumeratePhysicalDevices(gvk_instance, &n_physicalDevices, NULL);

	VkPhysicalDevice physicalDevices[n_physicalDevices];
	vkEnumeratePhysicalDevices(gvk_instance, &n_physicalDevices, physicalDevices);

	printf("n_physicalDevices: %i\n", n_physicalDevices);

	gvk_physicalDevice_p = &physicalDevices[0];

	uint32_t n_queueFamilyProperties = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(*gvk_physicalDevice_p, &n_queueFamilyProperties, NULL);

	VkQueueFamilyProperties queueFamilyProperties[n_queueFamilyProperties];
	vkGetPhysicalDeviceQueueFamilyProperties(*gvk_physicalDevice_p, &n_queueFamilyProperties, queueFamilyProperties);

	int queueFamilyIndex = -1;

	for(int i = 0; i < n_queueFamilyProperties; i++){
		if(queueFamilyProperties[i].queueCount > 0
		&& (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)){
			queueFamilyIndex = i;
			break;
		}
	}

	printf("n_queueFamilyProperties: %i\n", n_queueFamilyProperties);
	printf("queueFamilyIndex: %i\n", queueFamilyIndex);

	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
	deviceQueueCreateInfo.queueCount = 1;
	deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.ppEnabledLayerNames = validationLayerNames;
	deviceCreateInfo.enabledLayerCount = n_validationLayerNames;

	result = vkCreateDevice(*gvk_physicalDevice_p, &deviceCreateInfo, NULL, &gvk_device);

	printf("vkCreateDevice result: %i\n", result);

	VkQueue queue;
	vkGetDeviceQueue(gvk_device, queueFamilyIndex, 0, &queue);

	int n_numbers = 2 * 524288;
	int bufferSize = n_numbers * sizeof(int32_t);

	Buffer bufferA;
	Buffer_init(
		&bufferA,
		bufferSize,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
	);

	Buffer bufferB;
	Buffer_init(
		&bufferB,
		bufferSize,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
	);

	{
		int32_t *mappedMemory = NULL;
		vkMapMemory(gvk_device, bufferB.memory, 0, bufferSize, 0, (void **)&mappedMemory);
		for(int i = 0; i < n_numbers; i++){
			mappedMemory[i] = 420;
		}
		vkUnmapMemory(gvk_device, bufferB.memory);
	}
	{
		int32_t *mappedMemory = NULL;
		vkMapMemory(gvk_device, bufferA.memory, 0, bufferSize, 0, (void **)&mappedMemory);
		for(int i = 0; i < n_numbers; i++){
			mappedMemory[i] = 10;
		}
		vkUnmapMemory(gvk_device, bufferA.memory);
	}

	int n_descriptorSetLayoutBindings = 2;
	VkDescriptorSetLayout descriptorSetLayout;

	VkDescriptorSetLayoutBinding descriptorSetLayoutBindings[n_descriptorSetLayoutBindings] = {};
	{
		VkDescriptorSetLayoutBinding *descriptorSetLayoutBinding_p = &descriptorSetLayoutBindings[0];
		descriptorSetLayoutBinding_p->binding = 0;
		descriptorSetLayoutBinding_p->descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorSetLayoutBinding_p->descriptorCount = 1;
		descriptorSetLayoutBinding_p->stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	}
	{
		VkDescriptorSetLayoutBinding *descriptorSetLayoutBinding_p = &descriptorSetLayoutBindings[1];
		descriptorSetLayoutBinding_p->binding = 1;
		descriptorSetLayoutBinding_p->descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorSetLayoutBinding_p->descriptorCount = 1;
		descriptorSetLayoutBinding_p->stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	}

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.bindingCount = n_descriptorSetLayoutBindings;
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBindings;

	result = vkCreateDescriptorSetLayout(gvk_device, &descriptorSetLayoutCreateInfo, NULL, &descriptorSetLayout);
	printf("vkCreateDescriptorSetLayout result: %i\n", result);

	VkDescriptorPoolSize descriptorPoolSize = {};
	descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorPoolSize.descriptorCount = n_descriptorSetLayoutBindings;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.maxSets = 1;
	descriptorPoolCreateInfo.poolSizeCount = 1;
	descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;

	VkDescriptorPool descriptorPool;
	result = vkCreateDescriptorPool(gvk_device, &descriptorPoolCreateInfo, NULL, &descriptorPool);
	printf("vkCreateDescriptorPool result: %i\n", result);

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;

	VkDescriptorSet descriptorSet;
	result = vkAllocateDescriptorSets(gvk_device, &descriptorSetAllocateInfo, &descriptorSet);
	printf("vkAllocateDescriptorSets result: %i\n", result);

	{
		VkDescriptorBufferInfo descriptorBufferInfo = {};
		descriptorBufferInfo.buffer = bufferA.ID;
		descriptorBufferInfo.offset = 0;
		descriptorBufferInfo.range = bufferSize;

		VkWriteDescriptorSet writeDescriptorSet = {};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.dstSet = descriptorSet;
		writeDescriptorSet.dstBinding = 0;
		writeDescriptorSet.descriptorCount = 1;
		writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;

		vkUpdateDescriptorSets(gvk_device, 1, &writeDescriptorSet, 0, NULL);
	}
	{
		VkDescriptorBufferInfo descriptorBufferInfo = {};
		descriptorBufferInfo.buffer = bufferB.ID;
		descriptorBufferInfo.offset = 0;
		descriptorBufferInfo.range = bufferSize;

		VkWriteDescriptorSet writeDescriptorSet = {};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.dstSet = descriptorSet;
		writeDescriptorSet.dstBinding = 1;
		writeDescriptorSet.descriptorCount = 1;
		writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;

		vkUpdateDescriptorSets(gvk_device, 1, &writeDescriptorSet, 0, NULL);
	}

	uint32_t fileLength;
	uint32_t* code = readFile(fileLength, "shader.spv");
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.pCode = code;
	shaderModuleCreateInfo.codeSize = fileLength;

	VkShaderModule shaderModule;
	result = vkCreateShaderModule(gvk_device, &shaderModuleCreateInfo, NULL, &shaderModule);
	printf("vkCreateShaderModule result: %i\n", result);

	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
	pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	pipelineShaderStageCreateInfo.module = shaderModule;
	pipelineShaderStageCreateInfo.pName = "main";

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;

	VkPipelineLayout pipelineLayout;
	result = vkCreatePipelineLayout(gvk_device, &pipelineLayoutCreateInfo, NULL, &pipelineLayout);
	printf("vkCreatePipelineLayout result: %i\n", result);

	VkComputePipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stage = pipelineShaderStageCreateInfo;
	pipelineCreateInfo.layout = pipelineLayout;

	VkPipeline pipeline;
	result = vkCreateComputePipelines(gvk_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &pipeline);
	printf("vkCreateComputePipelines result: %i\n", result);
	
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = 0;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

	VkCommandPool commandPool;
	result = vkCreateCommandPool(gvk_device, &commandPoolCreateInfo, NULL, &commandPool);
	printf("vkCreateCommandPool result: %i\n", result);

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	result = vkAllocateCommandBuffers(gvk_device, &commandBufferAllocateInfo, &commandBuffer);
	printf("vkAllocateCommandBuffers result: %i\n", result);
	
	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	commandBufferBeginInfo.flags = 0;

	result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
	printf("vkBeginCommandBuffer result: %i\n", result);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet, 0, NULL);

	vkCmdDispatch(commandBuffer, n_numbers, 1, 1);

	result = vkEndCommandBuffer(commandBuffer);
	printf("vkEndCommandBuffer result: %i\n", result);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = 0;

	VkFence fence;
	result = vkCreateFence(gvk_device, &fenceCreateInfo, NULL, &fence);
	printf("vkCreateFence result: %i\n", result);

	result = vkQueueSubmit(queue, 1, &submitInfo, fence);
	printf("vkQueueSubmit result: %i\n", result);

	result = vkWaitForFences(gvk_device, 1, &fence, VK_TRUE, 100000000000);
	printf("vkWaitForFences result: %i\n", result);

	printf("done 1\n");

	vkResetFences(gvk_device, 1, &fence);

	result = vkQueueSubmit(queue, 1, &submitInfo, fence);
	printf("vkQueueSubmit result: %i\n", result);

	result = vkWaitForFences(gvk_device, 1, &fence, VK_TRUE, 100000000000);
	printf("vkWaitForFences result: %i\n", result);

	printf("done 2\n");

	/*
	{
		int32_t* mappedMemory = NULL;
		vkMapMemory(gvk_device, bufferA.memory, 0, bufferSize, 0, (void **)&mappedMemory);

		for(int i = 0; i < n_numbers; i++){
			printf("%i: %i\n", i, mappedMemory[i]);
		}
		printf("\n");

		vkUnmapMemory(gvk_device, bufferA.memory);
	}
	*/

	printf("hello vulkan!\n");

	return 0;

}
