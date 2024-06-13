#include "engine/gl3d.h"

typedef struct Face{
	unsigned int indices[9];
}Face;

void Model_initFromMeshData(Model *model_p, const unsigned char *mesh, int numberOfTriangles){

	//printf("%i\n", numberOfTriangles);

	int componentSize = 2 * sizeof(Vec3f) + sizeof(Vec2f);

	glGenBuffers(1, &model_p->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, model_p->VBO);
	glBufferData(GL_ARRAY_BUFFER, componentSize * 3 * numberOfTriangles, mesh, GL_STATIC_DRAW);

	glGenVertexArrays(1, &model_p->VAO);
	glBindVertexArray(model_p->VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	model_p->numberOfTriangles = numberOfTriangles;

}

void Model_initFromFile_mesh(Model *model_p, const char *path, AssetManager *assetManager_p){

	long int fileSize;
	char *data = getAssetData_mustFree(path, &fileSize, assetManager_p);

	int numberOfTriangles = fileSize / (sizeof(float) * 8 * 3);

	Model_initFromMeshData(model_p, (unsigned char *)data, numberOfTriangles);

	free(data);

}

unsigned char *generateMeshDataFromTriangleMesh(TriangleMesh triangleMesh, Vec2f *textureData){

	float *data = (float *)malloc(8 * sizeof(float) * 3 * triangleMesh.n_triangles);

	for(int i = 0; i < triangleMesh.n_triangles; i++){
		
		int trianglesIndex = i * 3;
		int dataIndex = i * 8 * 3;

		Vec3f point1 = triangleMesh.triangles[trianglesIndex + 0];
		Vec3f point2 = triangleMesh.triangles[trianglesIndex + 1];
		Vec3f point3 = triangleMesh.triangles[trianglesIndex + 2];

		Vec3f normal = normalize(cross(point2 - point1, point3 - point1));

		//normal = getVec3f(0.0, 1.0, 0.0);

		memcpy(data + dataIndex + 8 * 0, &point1, sizeof(Vec3f));
		memcpy(data + dataIndex + 8 * 0 + 5, &normal, sizeof(Vec3f));

		memcpy(data + dataIndex + 8 * 1, &point2, sizeof(Vec3f));
		memcpy(data + dataIndex + 8 * 1 + 5, &normal, sizeof(Vec3f));

		memcpy(data + dataIndex + 8 * 2, &point3, sizeof(Vec3f));
		memcpy(data + dataIndex + 8 * 2 + 5, &normal, sizeof(Vec3f));

		if(textureData != NULL){
			memcpy(data + dataIndex + 8 * 0 + 3, textureData + trianglesIndex + 0, sizeof(Vec2f));
			memcpy(data + dataIndex + 8 * 1 + 3, textureData + trianglesIndex + 1, sizeof(Vec2f));
			memcpy(data + dataIndex + 8 * 2 + 3, textureData + trianglesIndex + 2, sizeof(Vec2f));
		}

	}

	return (unsigned char *)data;

}

//COMMENTED OUT DUE TO std::vector
/*
void BoneModel_initFromFile(BoneModel *model_p, const char *path, AssetManager *assetManager_p){

	long int fileSize;
	char *data = getAssetData_mustFree(path, &fileSize, assetManager_p);

	int n_triangles = fileSize / (BONE_MODEL_COMPONENT_SIZE * 3);

	glGenBuffers(1, &model_p->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, model_p->VBO);
	glBufferData(GL_ARRAY_BUFFER, BONE_MODEL_COMPONENT_SIZE * 3 * n_triangles, data, GL_STATIC_DRAW);

	glGenVertexArrays(1, &model_p->VAO);
	glBindVertexArray(model_p->VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, BONE_MODEL_COMPONENT_SIZE, (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, BONE_MODEL_COMPONENT_SIZE, (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, BONE_MODEL_COMPONENT_SIZE, (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribIPointer(3, 4, GL_UNSIGNED_BYTE, BONE_MODEL_COMPONENT_SIZE, (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, BONE_MODEL_COMPONENT_SIZE, (void *)(8 * sizeof(float) + 4 * sizeof(unsigned char)));
	glEnableVertexAttribArray(4);

	model_p->n_triangles = n_triangles;

	free(data);

}
*/

/*
int BoneModel_getBoneIndexByName(BoneModel *model_p, const char *name){
	for(int i = 0; i < model_p->bones.size(); i++){
		if(strcmp(model_p->bones[i].name, name) == 0){
			return i;
		}
	}
	return -1;
}
*/

//COMMENTED OUT BECAUSE OF std::vector
/*
void PointMesh_initFromTriangleMesh(PointMesh *pointMesh_p, TriangleMesh triangleMesh){
	
	std::vector<Vec3f> points;

	float tolerance = 0.01;

	for(int i = 0; i < triangleMesh.n_triangles * 3; i++){

		bool exists = false;

		for(int j = 0; j < points.size(); j++){
			if(getMagVec3f(triangleMesh.triangles[i] - points[j]) < tolerance){
				exists = true;
				break;
			}
		}

		if(!exists){
			points.push_back(triangleMesh.triangles[i]);
		}
	
	}

	pointMesh_p->n_points = points.size();
	pointMesh_p->points = (Vec3f *)malloc(sizeof(Vec3f) * pointMesh_p->n_points);
	memcpy(pointMesh_p->points, &points[0], sizeof(Vec3f) * pointMesh_p->n_points);

	String_set(pointMesh_p->name, triangleMesh.name, STRING_SIZE);

}
*/

void Texture_init(Texture *texture_p, const char *name, unsigned char *data, int width, int height){

	String_set(texture_p->name, name, SMALL_STRING_SIZE);

	glGenTextures(1, &texture_p->ID);

	glBindTexture(GL_TEXTURE_2D, texture_p->ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void Texture_initAsDepthMap(Texture *texture_p, int width, int height){

    GLenum error1 = glGetError();

    float *tmpData = (float *)malloc(width * height * sizeof(float));

	glGenTextures(1, &texture_p->ID);
	glBindTexture(GL_TEXTURE_2D, texture_p->ID);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, tmpData);

    GLenum error2 = glGetError();

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    free(tmpData);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//Vec4f borderColor = getVec4f(1.0, 1.0, 1.0, 1.0);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float *)&borderColor);
	//float borderDepth = 0.0;
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BORDER_DEPTH, borderDepth);

}

void Texture_initAsColorMap(Texture *texture_p, int width, int height){

    unsigned char *tmpData = (unsigned char *)malloc(4 * width * height * sizeof(unsigned char));

    Texture_init(texture_p, "color-map", tmpData, width, height);
/*
	glGenTextures(1, &texture_p->ID);
	glBindTexture(GL_TEXTURE_2D, texture_p->ID);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, tmpData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/

    free(tmpData);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//Vec4f borderColor = getVec4f(1.0, 1.0, 1.0, 1.0);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float *)&borderColor);

}

void Texture_initFromFile(Texture *texture_p, const char *path, const char *name, AssetManager *assetManager_p){

	long int fileSize;
	char *fileData = (char *)getAssetData_mustFree(path, &fileSize, assetManager_p);

	int width, height, channels;
	unsigned char *data = stbi_load_from_memory((stbi_uc *)fileData, fileSize, &width, &height, &channels, 4);

	Texture_init(texture_p, name, data, width, height);

	free(fileData);
	free(data);

}

void Texture_initFromFileAsAlphaMap(Texture *texture_p, const char *path, const char *name){

	int width, height, channels;
	unsigned char *data = stbi_load(path, &width, &height, &channels, 4);

	unsigned char *alphaData = (unsigned char *)malloc(width * height * sizeof(unsigned char));

	for(int i = 0; i < width * height; i++){
		alphaData[i] = data[i * 4 + 3];
	}

	glGenTextures(1, &texture_p->ID);
	glBindTexture(GL_TEXTURE_2D, texture_p->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, alphaData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	String_set(texture_p->name, name, STRING_SIZE);

	//Texture_init(texture_p, name, data, width, height);

	free(data);
	free(alphaData);

}

void Texture_init3D(Texture *texture_p, const char *name, unsigned char *data, int width, int height, int depth){

	String_set(texture_p->name, name, SMALL_STRING_SIZE);

	glGenTextures(1, &texture_p->ID);

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_p->ID);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

/*
void TextureAtlas_init(TextureAtlas *textureAtlas_p, const char **pathsAndNames, int numberOfPaths){

	int atlasWidth = 0;
	int atlasHeight = 0;

	unsigned char *atlasData;
	unsigned char *textureData[numberOfPaths];
	Vec2f textureDataDimensions[numberOfPaths];

	for(int i = 0; i < numberOfPaths; i++){

		int width, height, channels;

		textureData[i] = stbi_load(pathsAndNames[i * 2], &width, &height, &channels, 4);

		textureDataDimensions[i].x = width;
		textureDataDimensions[i].y = height;

		atlasWidth += width;

		if(height > atlasHeight){
			atlasHeight = height;
		}

		SmallString name;
		String_set(name, pathsAndNames[i * 2 + 1], SMALL_STRING_SIZE);
		textureAtlas_p->names.push_back(name);

	}

	//create texture atlas data
	atlasData = (unsigned char *)malloc(4 * sizeof(unsigned char) * atlasWidth * atlasHeight);

	for(int y = 0; y < atlasHeight; y++){

		int x = 0;

		for(int i = 0; i < numberOfPaths; i++){

			if(y < (int)textureDataDimensions[i].y){

				unsigned char *dest = atlasData + 4 * (y * atlasWidth + x);
				unsigned char *src = textureData[i] + 4 * (y * (int)textureDataDimensions[i].y);
				int size = (int)textureDataDimensions[i].x * 4 * sizeof(unsigned char);

				memcpy(dest, src, size);

			}
				
			x += textureDataDimensions[i].x;

		}
	
	}

	//set texture coordinates
	{
		float x = 0.0;

		for(int i = 0; i < numberOfPaths; i++){

			Vec4f coordinates = getVec4f(x / atlasWidth, 1.0 - textureDataDimensions[i].y / atlasHeight, textureDataDimensions[i].x / atlasWidth, textureDataDimensions[i].y / atlasHeight);

			textureAtlas_p->textureCoordinates.push_back(coordinates);
			
			x += textureDataDimensions[i].x;
		
		}
	}

	Texture_init(&textureAtlas_p->texture, "atlas", atlasData, atlasWidth, atlasHeight);

	//free data
	free(atlasData);
	for(int i = 0; i < numberOfPaths; i++){
		free(textureData[i]);
	}

}
*/

void TextureBuffer_init(TextureBuffer *textureBuffer_p, void *data, int size){

	/*
	glGenBuffers(1, &textureBuffer_p->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer_p->VBO);
	//glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	glGenTextures(1, &textureBuffer_p->TB);
	glBindTexture(GL_TEXTURE_BUFFER, textureBuffer_p->TB);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, textureBuffer_p->VBO);
	*/

}

void TextureBuffer_update(TextureBuffer *textureBuffer_p, int offset, int size, void *data){

	/*
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer_p->VBO);

	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	*/
	
}

void TextureBuffer_initAsVec4fArray(TextureBuffer *textureBuffer_p, Vec4f *vectors, int n_vectors){
	/*
	textureBuffer_p->n_elements = n_vectors;
	textureBuffer_p->elementSize = sizeof(Vec4f);

	glGenBuffers(1, &textureBuffer_p->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer_p->VBO);
	glBufferData(GL_ARRAY_BUFFER, textureBuffer_p->n_elements * textureBuffer_p->elementSize, vectors, GL_STATIC_DRAW);

	glGenTextures(1, &textureBuffer_p->TB);
	glBindTexture(GL_TEXTURE_BUFFER, textureBuffer_p->TB);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, textureBuffer_p->VBO);
	*/

}

void TextureBuffer_initAsMat4fArray(TextureBuffer *textureBuffer_p, Mat4f *matrices, int n_matrices, bool compress){

	/*
	textureBuffer_p->n_elements = n_matrices;
	textureBuffer_p->elementSize = sizeof(Mat4f);

	glGenBuffers(1, &textureBuffer_p->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer_p->VBO);
	glBufferData(GL_ARRAY_BUFFER, textureBuffer_p->n_elements * textureBuffer_p->elementSize, matrices, GL_STATIC_DRAW);

	glGenTextures(1, &textureBuffer_p->TB);
	glBindTexture(GL_TEXTURE_BUFFER, textureBuffer_p->TB);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, textureBuffer_p->VBO);
	*/

}

void Shader_init(Shader *shader_p, const char *name, const char *vertexShaderPath, const char *fragmentShaderPath, AssetManager *assetManager_p){

	unsigned int vertexShader = getCompiledShader(vertexShaderPath, GL_VERTEX_SHADER, assetManager_p);
	unsigned int fragmentShader = getCompiledShader(fragmentShaderPath, GL_FRAGMENT_SHADER, assetManager_p);

	shader_p->ID = glCreateProgram();
	glAttachShader(shader_p->ID, vertexShader);
	glAttachShader(shader_p->ID, fragmentShader);
	glLinkProgram(shader_p->ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

    GLint isLinked = 0;
    glGetProgramiv(shader_p->ID, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shader_p->ID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        GLchar infoLog[maxLength];
        //std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shader_p->ID, maxLength, &maxLength, infoLog);

        int x = 0;

    }

	String_set(shader_p->name, name, STRING_SIZE);

}

/*
bool checkTriangleMeshTriangleMeshCollision(TriangleMesh mesh1, TriangleMesh mesh2, Mat4f matrix1, Mat4f matrix2, Vec3f *output_collisionPoint, Vec3f *output_collisionNormal){

	Vec3f collisionPoint = getVec3f(0.0, 0.0, 0.0);
	Vec3f collisionNormal = getVec3f(0.0, 0.0, 0.0);
	int n_hits = 0;

	for(int i = 0; i < mesh1.n_triangles; i++){

		Vec3f a1 = mulVec3fMat4f(mesh1.triangles[i * 3 + 0], matrix1, 1.0);
		Vec3f a2 = mulVec3fMat4f(mesh1.triangles[i * 3 + 1], matrix1, 1.0);
		Vec3f a3 = mulVec3fMat4f(mesh1.triangles[i * 3 + 2], matrix1, 1.0);
		Vec3f aNormal = normalize(cross(a2 - a1, a3 - a1));

		float aRadius = max(dot(a1 - a2, a1 - a2), dot(a1 - a3, a1 - a3));

		for(int j = 0; j < mesh2.n_triangles; j++){

			Vec3f b1 = mulVec3fMat4f(mesh2.triangles[j * 3 + 0], matrix2, 1.0);
			Vec3f b2 = mulVec3fMat4f(mesh2.triangles[j * 3 + 1], matrix2, 1.0);
			Vec3f b3 = mulVec3fMat4f(mesh2.triangles[j * 3 + 2], matrix2, 1.0);
			Vec3f bNormal = normalize(cross(b2 - b1, b3 - b1));

			if(dot(a1 - b1, a1 - b1) > aRadius
			&& dot(a1 - b2, a1 - b2) > aRadius
			&& dot(a1 - b3, a1 - b3) > aRadius){
				continue;
			}

			Vec3f aPoints[3];
			int n_aPoints = 0;

			float t = (dot(b1, bNormal) - dot(a1, bNormal)) / dot(a2 - a1, bNormal);
			if(t >= 0.0 && t <= 1.0){
				aPoints[n_aPoints] = a1 + (a2 - a1) * t;
				n_aPoints++;
			}

			t = (dot(b1, bNormal) - dot(a2, bNormal)) / dot(a3 - a2, bNormal);
			if(t >= 0.0 && t <= 1.0){
				aPoints[n_aPoints] = a2 + (a3 - a2) * t;
				n_aPoints++;
			}

			t = (dot(b1, bNormal) - dot(a3, bNormal)) / dot(a1 - a3, bNormal);
			if(t >= 0.0 && t <= 1.0){
				aPoints[n_aPoints] = a3 + (a1 - a3) * t;
				n_aPoints++;
			}

			Vec3f bPoints[3];
			int n_bPoints = 0;

			t = (dot(a1, aNormal) - dot(b1, aNormal)) / dot(b2 - b1, aNormal);
			if(t >= 0.0 && t <= 1.0){
				bPoints[n_bPoints] = b1 + (b2 - b1) * t;
				n_bPoints++;
			}

			t = (dot(a1, aNormal) - dot(b2, aNormal)) / dot(b3 - b2, aNormal);
			if(t >= 0.0 && t <= 1.0){
				bPoints[n_bPoints] = b2 + (b3 - b2) * t;
				n_bPoints++;
			}

			t = (dot(a1, aNormal) - dot(b3, aNormal)) / dot(b1 - b3, aNormal);
			if(t >= 0.0 && t <= 1.0){
				bPoints[n_bPoints] = b3 + (b1 - b3) * t;
				n_bPoints++;
			}

			if(n_aPoints == 2
			&& n_bPoints == 2){
				if(dot(aPoints[0] - bPoints[0], aPoints[0] - bPoints[1]) < 0.0
				|| dot(aPoints[1] - bPoints[0], aPoints[1] - bPoints[1]) < 0.0
				|| dot(bPoints[0] - aPoints[0], bPoints[0] - aPoints[1]) < 0.0
				|| dot(bPoints[1] - aPoints[0], bPoints[1] - aPoints[1]) < 0.0){

					collisionPoint += (bPoints[0] + bPoints[1]) / 2.0;

					if(dot(rigidBody_p->velocity * -1.0, aNormal) > dot(rigidBody_p->velocity * -1.0, collisionNormal)
					|| getMagVec3f(collisionNormal) < 0.01){
						collisionNormal = aNormal;
					}

					n_hits++;

				}
			}
			
		}
	
	}

	collisionPoint = collisionPoint / (float)n_hits;
	collisionNormal = normalize(collisionNormal);

	if(output_collisionPoint != NULL){
		*output_collisionPoint = collisionPoint;
	}
	if(output_collisionNormal != NULL){
		*output_collisionNormal = collisionNormal;
	}

	return n_hits > 0;

}
*/

Vec3f PointMesh_GJKSupport(PointMesh *pointMesh_p, Mat4f transformations, Vec3f direction){

	Vec3f maxPoint = mulVec3fMat4f(pointMesh_p->points[0], transformations, 1.0);
	float maxProduct = dot(maxPoint, direction);

	for(int i = 1; i < pointMesh_p->n_points; i++){

		Vec3f point = mulVec3fMat4f(pointMesh_p->points[i], transformations, 1.0);
		float product = dot(point, direction);

		if(product > maxProduct){
			maxPoint = point;
			maxProduct = product;
		}

	}

	return maxPoint;

}

//free functions
void Model_free(Model *model_p){
	glDeleteBuffers(1, &model_p->VBO);
	glDeleteVertexArrays(1, &model_p->VAO);
}

void TextureBuffer_free(TextureBuffer *textureBuffer_p){
	glDeleteBuffers(1, &textureBuffer_p->VBO);
	glDeleteTextures(1, &textureBuffer_p->TB);
}

void Texture_free(Texture *texture_p){
	glDeleteTextures(1, &texture_p->ID);
}

void Shader_free(Shader *shader_p){
	glDeleteProgram(shader_p->ID);
}

//uniform functions
void GL3D_uniformMat2f(unsigned int shaderProgram, const char *locationName, Mat2f m){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);

	glUniformMatrix2fv(location, 1, GL_FALSE, (float *)m.values);

}

void GL3D_uniformMat3f(unsigned int shaderProgram, const char *locationName, Mat3f m){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);

	glUniformMatrix3fv(location, 1, GL_FALSE, (float *)m.values);

}

void GL3D_uniformMat4f(unsigned int shaderProgram, const char *locationName, Mat4f m){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);

	glUniformMatrix4fv(location, 1, GL_FALSE, (float *)m.values);

}

void GL3D_uniformMat4fArray(unsigned int shaderProgram, const char *locationName, Mat4f *matrices, int n_matrices){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);

	glUniformMatrix4fv(location, n_matrices, GL_FALSE, (float *)matrices);

}

void GL3D_uniformVec2f(unsigned int shaderProgram, const char *locationName, Vec2f v){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);

	glUniform2f(location, v.x, v.y);

}

void GL3D_uniformVec3f(unsigned int shaderProgram, const char *locationName, Vec3f v){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);

	glUniform3f(location, v.x, v.y, v.z);

}

void GL3D_uniformVec4f(unsigned int shaderProgram, const char *locationName, Vec4f v){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);

	glUniform4f(location, v.x, v.y, v.z, v.w);

}

void GL3D_uniformInt(unsigned int shaderProgram, const char *locationName, int x){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);

	glUniform1i(location, x);

}

void GL3D_uniformFloat(unsigned int shaderProgram, const char *locationName, float x){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);

	glUniform1f(location, x);

}

void GL3D_uniformTexture(unsigned int shaderProgram, const char *locationName, unsigned int locationIndex, unsigned int textureID){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);
	glUniform1i(location, locationIndex);
	glActiveTexture(GL_TEXTURE0 + locationIndex);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

void GL3D_uniformTexture3D(unsigned int shaderProgram, const char *locationName, unsigned int locationIndex, unsigned int textureID){

	unsigned int location = glGetUniformLocation(shaderProgram, locationName);
	glUniform1i(location, locationIndex);
	glActiveTexture(GL_TEXTURE0 + locationIndex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);

}

void GL3D_uniformTextureBuffer(unsigned int shaderProgram, const char *locationName, unsigned int locationIndex, unsigned int textureID){

	/*
	unsigned int location = glGetUniformLocation(shaderProgram, locationName);
	glUniform1i(location, locationIndex);
	glActiveTexture(GL_TEXTURE0 + locationIndex);
	glBindTexture(GL_TEXTURE_BUFFER, textureID);
	*/

}

bool checkClosestLineTriangleMeshIntersection(Vec3f lineOrigin, Vec3f lineDirection, TriangleMesh triangleMesh, Vec3f triangleMeshPosition, float triangleMeshScale, Vec3f *outputIntersectionPoint_p, int *outputTriangleIndex_p){

	/*
	Vec3f intersectionPoint;
	int intersectionTriangleIndex;
	bool intersected = false;
	float closestDistance;

	for(int i = 0; i < triangleMesh.n_triangles; i++){

		Vec3f checkPoint;

		if(checkLineToTriangleIntersectionVec3f(
			lineOrigin,
			lineOrigin + lineDirection,
			triangleMeshPosition + triangleMesh.triangles[i * 3 + 0] * triangleMeshScale,
			triangleMeshPosition + triangleMesh.triangles[i * 3 + 1] * triangleMeshScale,
			triangleMeshPosition + triangleMesh.triangles[i * 3 + 2] * triangleMeshScale,
			&checkPoint
		) && (!intersected
		|| dot(checkPoint - lineOrigin, checkPoint - lineOrigin) < closestDistance)){
			intersectionPoint = checkPoint;
			intersected = true;
			intersectionTriangleIndex = i;
			closestDistance = dot(intersectionPoint - lineOrigin, intersectionPoint - lineOrigin);
		}
	}

	if(outputIntersectionPoint_p != NULL){
		*outputIntersectionPoint_p = intersectionPoint;
	}
	if(outputTriangleIndex_p != NULL){
		*outputTriangleIndex_p = intersectionTriangleIndex;
	}

	return intersected;
	*/
	return false;

}
