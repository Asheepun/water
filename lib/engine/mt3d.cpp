#ifdef __APPLE__

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "engine/mt3d.h"
#include "engine/files.h"
#include "engine/engine.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void Model_init(Model *model_p, Vec3f *positions, Vec2f *textureCoords, Vec3f *normals, int n_vertices){

	Vec4f *alignedPositions = (Vec4f *)malloc(n_vertices * sizeof(Vec4f));
	Vec2f *alignedTextureCoords = (Vec2f *)malloc(n_vertices * sizeof(Vec2f));
	Vec4f *alignedNormals = (Vec4f *)malloc(n_vertices * sizeof(Vec4f));

	for(int i = 0; i < n_vertices; i++){
		alignedPositions[i].x = positions[i].x;
		alignedPositions[i].y = positions[i].y;
		alignedPositions[i].z = positions[i].z;
		alignedPositions[i].w = 0.0;
	}

	for(int i = 0; i < n_vertices; i++){
		alignedTextureCoords[i].x = textureCoords[i].x;
		alignedTextureCoords[i].y = textureCoords[i].y;
		//alignedTextureCoords[i].z = 0.0;
		//alignedTextureCoords[i].w = 0.0;
	}

	for(int i = 0; i < n_vertices; i++){
		alignedNormals[i].x = normals[i].x;
		alignedNormals[i].y = normals[i].y;
		alignedNormals[i].z = normals[i].z;
		alignedNormals[i].w = 0.0;
	}

	model_p->n_vertices = n_vertices;

	MTL::Buffer *sharedPositionsBuffer_p = mt_device_p->newBuffer(n_vertices * sizeof(Vec4f), MTL::ResourceStorageModeShared);
	MTL::Buffer *sharedTextureCoordsBuffer_p = mt_device_p->newBuffer(n_vertices * sizeof(Vec2f), MTL::ResourceStorageModeShared);
	MTL::Buffer *sharedNormalsBuffer_p = mt_device_p->newBuffer(n_vertices * sizeof(Vec4f), MTL::ResourceStorageModeShared);

	memcpy(sharedPositionsBuffer_p->contents(), alignedPositions, n_vertices * sizeof(Vec4f));
	memcpy(sharedTextureCoordsBuffer_p->contents(), alignedTextureCoords, n_vertices * sizeof(Vec2f));
	memcpy(sharedNormalsBuffer_p->contents(), alignedNormals, n_vertices * sizeof(Vec4f));

	model_p->positionsBuffer_p = mt_device_p->newBuffer(n_vertices * sizeof(Vec4f), MTL::ResourceStorageModePrivate);
	model_p->textureCoordsBuffer_p = mt_device_p->newBuffer(n_vertices * sizeof(Vec2f), MTL::ResourceStorageModePrivate);
	model_p->normalsBuffer_p = mt_device_p->newBuffer(n_vertices * sizeof(Vec4f), MTL::ResourceStorageModePrivate);

	MTL::CommandBuffer *commandBuffer_p = mt_commandQueue_p->commandBuffer();
	MTL::BlitCommandEncoder *blitCommandEncoder_p = commandBuffer_p->blitCommandEncoder();

	blitCommandEncoder_p->copyFromBuffer(sharedPositionsBuffer_p, 0, model_p->positionsBuffer_p, 0, n_vertices * sizeof(Vec4f));
	blitCommandEncoder_p->copyFromBuffer(sharedTextureCoordsBuffer_p, 0, model_p->textureCoordsBuffer_p, 0, n_vertices * sizeof(Vec2f));
	blitCommandEncoder_p->copyFromBuffer(sharedNormalsBuffer_p, 0, model_p->normalsBuffer_p, 0, n_vertices * sizeof(Vec4f));

	blitCommandEncoder_p->endEncoding();

	commandBuffer_p->commit();

	sharedPositionsBuffer_p->release();
	sharedTextureCoordsBuffer_p->release();
	sharedNormalsBuffer_p->release();

	free(alignedPositions);
	free(alignedTextureCoords);
	free(alignedNormals);

}

void Model_initFromFile_mesh(Model *model_p, const char *path, const char *name, AssetManager *assetManager_p){

	long int fileSize;
	char *data = getFileData_mustFree(path, &fileSize);

	int componentSize = sizeof(Vec3f) + sizeof(Vec2f) + sizeof(Vec3f);
	int n_vertices = fileSize / componentSize;

	Vec3f *positions = (Vec3f *)malloc(n_vertices * sizeof(Vec3f));
	Vec2f *textureCoords = (Vec2f *)malloc(n_vertices * sizeof(Vec2f));
	Vec3f *normals = (Vec3f *)malloc(n_vertices * sizeof(Vec3f));

	for(int i = 0; i < n_vertices; i++){

		memcpy(positions + i, data + i * componentSize, sizeof(Vec3f));
		memcpy(textureCoords + i, data + i * componentSize + sizeof(Vec3f), sizeof(Vec2f));
		memcpy(normals + i, data + i * componentSize + sizeof(Vec3f) + sizeof(Vec2f), sizeof(Vec3f));

	}

	Model_init(model_p, positions, textureCoords, normals, n_vertices);

	free(data);
	free(positions);
	free(textureCoords);
	free(normals);

}

void Texture_init(Texture *texture_p, const char *name, unsigned char *data, int width, int height){

	texture_p->name = string(name);

	MTL::TextureDescriptor *textureDescriptor_p = MTL::TextureDescriptor::alloc()->init();

	textureDescriptor_p->setWidth(width);
	textureDescriptor_p->setHeight(height);
	textureDescriptor_p->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
	textureDescriptor_p->setTextureType(MTL::TextureType2D);
	textureDescriptor_p->setStorageMode(MTL::StorageModePrivate);
	textureDescriptor_p->setUsage(MTL::ResourceUsageSample | MTL::ResourceUsageRead);

	texture_p->pointer = mt_device_p->newTexture(textureDescriptor_p);

	MTL::Buffer *sharedBuffer_p = mt_device_p->newBuffer(width * height * 4, MTL::ResourceStorageModeShared);
	memcpy(sharedBuffer_p->contents(), data, width * height * 4);

	MTL::CommandBuffer *commandBuffer_p = mt_commandQueue_p->commandBuffer();
	MTL::BlitCommandEncoder *blitCommandEncoder_p = commandBuffer_p->blitCommandEncoder();

	blitCommandEncoder_p->copyFromBuffer(sharedBuffer_p, 0, width * 4, 0, MTL::Size::Make(width, height, 1), texture_p->pointer, 0, 0, MTL::Origin::Make(0, 0, 0));

	blitCommandEncoder_p->endEncoding();

	commandBuffer_p->commit();

	textureDescriptor_p->release();
	sharedBuffer_p->release();

}

void Texture_initFromFile(Texture *texture_p, const char *path, const char *name, AssetManager *assetManager_p){

	long int fileSize;
	char *fileData = getFileData_mustFree(path, &fileSize);

	int width, height, channels;
	unsigned char *data = stbi_load_from_memory((stbi_uc *)fileData, (int)fileSize, &width, &height, &channels, 4);

	Texture_init(texture_p, name, data, width, height);

	free(fileData);
	free(data);

}

void Texture_initAs2DTextureArray(Texture *texture_p, const char *name, unsigned char *data, int width, int height, int depth){

	texture_p->name = string(name);

	MTL::TextureDescriptor *textureDescriptor_p = MTL::TextureDescriptor::alloc()->init();

	textureDescriptor_p->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
	textureDescriptor_p->setTextureType(MTL::TextureType2DArray);
	textureDescriptor_p->setStorageMode(MTL::StorageModePrivate);
	textureDescriptor_p->setUsage(MTL::ResourceUsageSample | MTL::ResourceUsageRead);
	textureDescriptor_p->setWidth(width);
	textureDescriptor_p->setHeight(height);
	textureDescriptor_p->setArrayLength(depth);

	texture_p->pointer = mt_device_p->newTexture(textureDescriptor_p);

	//MTL::Buffer *sharedBuffer_p = mt_device_p->newBuffer(width * height * depth * 4, MTL::ResourceStorageModeShared);
	//memcpy(sharedBuffer_p->contents(), data, width * height * depth * 4);

	MTL::CommandBuffer *commandBuffer_p = mt_commandQueue_p->commandBuffer();
	MTL::BlitCommandEncoder *blitCommandEncoder_p = commandBuffer_p->blitCommandEncoder();

	Texture slices[depth];

	for(int i = 0; i < depth; i++){
		Texture_init(&slices[i], "slice", data + i * width * height * 4, width, height);
		blitCommandEncoder_p->copyFromTexture(slices[i].pointer, 0, 0, texture_p->pointer, i, 0, 1, 1);
	}

	//blitCommandEncoder_p->copyFromBuffer(sharedBuffer_p, 0, width * 4, width * height * 4, MTL::Size::Make(width, height, depth), texture_p->pointer, 0, 0, MTL::Origin::Make(0, 0, 0));

	blitCommandEncoder_p->endEncoding();

	commandBuffer_p->commit();

	textureDescriptor_p->release();
	//sharedBuffer_p->release();

	for(int i = 0; i < depth; i++){
		slices[i].pointer->release();
	}

}

void Shader_init(Shader *shader_p, const char *name, const char *path, const char *vertexFunction, const char *fragmentFunction, AssetManager *assetManager_p){

	shader_p->name = string(name);

	long int fileSize;
	char *fileData = getFileData_mustFree(path, &fileSize);

	using NS::StringEncoding::UTF8StringEncoding;

	NS::Error *error_p = NULL;
	MTL::Library *library_p = mt_device_p->newLibrary( NS::String::string(fileData, UTF8StringEncoding), NULL, &error_p);
	if(!library_p){
		printf("%s", error_p->localizedDescription()->utf8String());
	}

	shader_p->vertexFunction_p = library_p->newFunction( NS::String::string(vertexFunction, UTF8StringEncoding) );
	shader_p->fragmentFunction_p = library_p->newFunction( NS::String::string(fragmentFunction, UTF8StringEncoding) );

	MTL::RenderPipelineDescriptor *pipelineDescriptor_p = MTL::RenderPipelineDescriptor::alloc()->init();
	pipelineDescriptor_p->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
	pipelineDescriptor_p->colorAttachments()->object(0)->setBlendingEnabled(true);
	pipelineDescriptor_p->colorAttachments()->object(0)->setRgbBlendOperation(MTL::BlendOperationAdd);
	pipelineDescriptor_p->colorAttachments()->object(0)->setAlphaBlendOperation(MTL::BlendOperationAdd);
	pipelineDescriptor_p->colorAttachments()->object(0)->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
	pipelineDescriptor_p->colorAttachments()->object(0)->setDestinationAlphaBlendFactor(MTL::BlendFactorOne);
	pipelineDescriptor_p->colorAttachments()->object(0)->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
	pipelineDescriptor_p->colorAttachments()->object(0)->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);

	pipelineDescriptor_p->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);

	pipelineDescriptor_p->setVertexFunction(shader_p->vertexFunction_p);
	pipelineDescriptor_p->setFragmentFunction(shader_p->fragmentFunction_p);

	//pipelineDescriptor_p->colorAttachments()->object(1)->setPixelFormat(MTL::PixelFormat::PixelFormatR32Float);

	pipelineDescriptor_p->colorAttachments()->object(1)->setPixelFormat(MTL::PixelFormat::PixelFormatInvalid);

	shader_p->PSO_p = mt_device_p->newRenderPipelineState(pipelineDescriptor_p, &error_p);

	library_p->release();
	free(fileData);

}

void mt_draw_copyToPrivateBuffer(MTL::Buffer *privateBuffer_p, MTL::Buffer *sharedBuffer_p, void *data, int size){

	if(size <= 0){
		return;
	}

	memcpy(sharedBuffer_p->contents(), data, size);

	//MTL::CommandBuffer *commandBuffer_p = mt_commandQueue_p->commandBuffer();
	MTL::BlitCommandEncoder *blitCommandEncoder_p = mt_drawCommandBuffer_p->blitCommandEncoder();

	blitCommandEncoder_p->copyFromBuffer(sharedBuffer_p, 0, privateBuffer_p, 0, size);

	blitCommandEncoder_p->endEncoding();
	//commandBuffer_p->commit();

}

void TripleBuffer_init(TripleBuffer *buffer_p, int size){
	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
		buffer_p->pointers[i] = mt_device_p->newBuffer(size, MTL::ResourceStorageModePrivate);
		buffer_p->sharedPointers[i] = mt_device_p->newBuffer(size, MTL::ResourceStorageModeShared);
	}
}

void Texture_free(Texture *texture_p){
	//do nothing yet
}

void Shader_free(Shader *shader_p){
	//do nothing yet
}

void Model_free(Model *model_p){
	//do nothing yet
}

#endif
