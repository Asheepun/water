#ifdef __APPLE__

#ifndef MT3D_H_
#define MT3D_H_

//#define NS_PRIVATE_IMPLEMENTATION
//#define MTL_PRIVATE_IMPLEMENTATION
//#define MTK_PRIVATE_IMPLEMENTATION
//#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "engine/geometry.h"
#include "engine/strings.h"
#include "engine/assetManager.h"

static const int MAX_FRAMES_IN_FLIGHT = 3;

struct Model{
	MTL::Buffer *positionsBuffer_p;
	MTL::Buffer *textureCoordsBuffer_p;
	MTL::Buffer *normalsBuffer_p;
	int n_vertices;
};

struct Texture{
	String name;
	MTL::Texture *pointer;
};

struct Shader{
	String name;
	MTL::Function *vertexFunction_p;
	MTL::Function *fragmentFunction_p;
	MTL::RenderPipelineState *PSO_p;
};

struct TripleBuffer{
	MTL::Buffer *pointers[MAX_FRAMES_IN_FLIGHT];
	MTL::Buffer *sharedPointers[MAX_FRAMES_IN_FLIGHT];
};

extern MTK::View *mt_view_p;
extern MTL::Device *mt_device_p;
extern MTL::CommandQueue *mt_commandQueue_p;
extern MTL::CommandBuffer *mt_drawCommandBuffer_p;
extern dispatch_semaphore_t mt_semaphore;

extern int mt_frameIndex;

void Model_init(Model *, Vec3f *, Vec2f *, Vec3f *, int);
void Model_initFromFile_mesh(Model *, const char *, const char *, AssetManager *);

void Texture_init(Texture *, const char *, unsigned char *, int, int);
void Texture_initFromFile(Texture *, const char *, const char *,AssetManager *);
void Texture_initAs2DTextureArray(Texture *, const char *, unsigned char *, int, int, int);

void Shader_init(Shader *, const char *, const char *, const char *, const char *, AssetManager *);

void mt_draw_copyToPrivateBuffer(MTL::Buffer *, MTL::Buffer *, void *, int);

void TripleBuffer_init(TripleBuffer *, int);

void Texture_free(Texture *);
void Shader_free(Shader *);
void Model_free(Model *);

#endif

#endif
