int N_NUMBERS = 2 * 524288;
const int WORK_GROUP_SIZE = 128;

Buffer bufferA;
Buffer bufferB;

Vec2f *positions;
Vec2f *velocities;

VkCommandBuffer commandBuffer;
VkFence fence;

void Engine_start(){

	positions = (Vec2f *)malloc(N_NUMBERS * sizeof(float));
	velocities = (Vec2f *)malloc(N_NUMBERS * sizeof(float));

	gvk_init();

	Buffer_init(
		&bufferA,
		N_NUMBERS * sizeof(int32_t),
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		//VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	Buffer_init(
		&bufferB,
		N_NUMBERS * sizeof(int32_t),
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		//VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
	);

	/*
	{
		int32_t *mappedMemory = NULL;
		vkMapMemory(gvk_device, bufferA.memory, 0, N_NUMBERS * sizeof(int32_t), 0, (void **)&mappedMemory);
		for(int i = 0; i < N_NUMBERS; i++){
			mappedMemory[i] = i;
		}
		vkUnmapMemory(gvk_device, bufferA.memory);
	}
	{
		int32_t *mappedMemory = NULL;
		vkMapMemory(gvk_device, bufferB.memory, 0, N_NUMBERS * sizeof(int32_t), 0, (void **)&mappedMemory);
		for(int i = 0; i < N_NUMBERS; i++){
			mappedMemory[i] = i * 2;
		}
		vkUnmapMemory(gvk_device, bufferB.memory);
	}
	*/

	//specify shader description
	Array<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;
	descriptorSetLayoutBindings.init();

	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = 0;
		descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		descriptorSetLayoutBindings.push(descriptorSetLayoutBinding);
	}
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = 1;
		descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		descriptorSetLayoutBindings.push(descriptorSetLayoutBinding);
	}

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.bindingCount = descriptorSetLayoutBindings.length;
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBindings.elements;

	//allocate descriptor set
	VkDescriptorSetLayout descriptorSetLayout;
	VkResult result = vkCreateDescriptorSetLayout(gvk_device, &descriptorSetLayoutCreateInfo, NULL, &descriptorSetLayout);
	if(result != 0) printf("vkCreateDescriptorSetLayout result: %i\n", result);

	VkDescriptorPoolSize descriptorPoolSize = {};
	descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorPoolSize.descriptorCount = descriptorSetLayoutBindings.length;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.maxSets = 1;
	descriptorPoolCreateInfo.poolSizeCount = 1;
	descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;

	VkDescriptorPool descriptorPool;
	result = vkCreateDescriptorPool(gvk_device, &descriptorPoolCreateInfo, NULL, &descriptorPool);
	if(result != 0) printf("vkCreateDescriptorPool result: %i\n", result);

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;

	VkDescriptorSet descriptorSet;
	result = vkAllocateDescriptorSets(gvk_device, &descriptorSetAllocateInfo, &descriptorSet);
	if(result != 0) printf("vkAllocateDescriptorSets result: %i\n", result);

	//bind buffers to descriptor set
	{
		VkDescriptorBufferInfo descriptorBufferInfo = {};
		descriptorBufferInfo.buffer = bufferA.ID;
		descriptorBufferInfo.offset = 0;
		descriptorBufferInfo.range = N_NUMBERS * sizeof(int32_t);

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
		descriptorBufferInfo.range = N_NUMBERS * sizeof(int32_t);

		VkWriteDescriptorSet writeDescriptorSet = {};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.dstSet = descriptorSet;
		writeDescriptorSet.dstBinding = 1;
		writeDescriptorSet.descriptorCount = 1;
		writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;

		vkUpdateDescriptorSets(gvk_device, 1, &writeDescriptorSet, 0, NULL);
	}

	//read shader and create pipeline
	long int fileSize;
	char *data = getFileData_mustFree("shader.spv", &fileSize);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.pCode = (uint32_t *)data;
	shaderModuleCreateInfo.codeSize = fileSize;

	VkShaderModule shaderModule;
	result = vkCreateShaderModule(gvk_device, &shaderModuleCreateInfo, NULL, &shaderModule);
	if(result != 0) printf("vkCreateShaderModule result: %i\n", result);

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
	if(result != 0) printf("vkCreatePipelineLayout result: %i\n", result);

	VkComputePipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stage = pipelineShaderStageCreateInfo;
	pipelineCreateInfo.layout = pipelineLayout;

	VkPipeline pipeline;
	result = vkCreateComputePipelines(gvk_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &pipeline);
	if(result != 0) printf("vkCreateComputePipelines result: %i\n", result);

	//setup command buffer
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = 0;
	commandPoolCreateInfo.queueFamilyIndex = gvk_queueFamilyIndex;

	VkCommandPool commandPool;
	result = vkCreateCommandPool(gvk_device, &commandPoolCreateInfo, NULL, &commandPool);
	if(result != 0) printf("vkCreateCommandPool result: %i\n", result);

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;

	result = vkAllocateCommandBuffers(gvk_device, &commandBufferAllocateInfo, &commandBuffer);
	if(result != 0) printf("vkAllocateCommandBuffers result: %i\n", result);

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	commandBufferBeginInfo.flags = 0;

	vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet, 0, NULL);

	vkCmdDispatch(commandBuffer, N_NUMBERS / WORK_GROUP_SIZE, 1, 1);

	vkEndCommandBuffer(commandBuffer);

	//init fence
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = 0;

	result = vkCreateFence(gvk_device, &fenceCreateInfo, NULL, &fence);
	if(result != 0) printf("vkCreateFence result: %i\n", result);

}

void Engine_initDrawing(){

}

void Engine_update(){

	if(Engine_keys[ENGINE_KEY_Q].downed){
		Engine_quit();
	}

	/*
	{
		long long st = getTime_us();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(gvk_queue, 1, &submitInfo, fence);

		vkWaitForFences(gvk_device, 1, &fence, VK_TRUE, 100000000000);

		vkResetFences(gvk_device, 1, &fence);

		long long et = getTime_us();

		printf("time: %lli us\n", et - st);
	}
	*/

	{
		long long st = getTime_us();

		for(int i = 0; i < N_NUMBERS / 2; i++){
			positions[i] += velocities[i];
		}

		long long et = getTime_us();

		printf("time: %lli us\n", et - st);
	}
	
	//print(Engine_deltaTime);

}

void Engine_draw(){

}

void Engine_finish(){

}
