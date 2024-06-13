enum Type3D{
	TYPE_I8,
	TYPE_I16,
	TYPE_F32,
	TYPE_F64,
};

typedef struct Model{
	char name[STRING_SIZE];
	unsigned int VBO;
	unsigned int VAO;
	unsigned int numberOfTriangles;
}Model;

struct BoneModel{
	char name[STRING_SIZE];
	unsigned int VBO;
	unsigned int VAO;
	unsigned int n_triangles;
	//std::vector<Bone> bones;
	//std::vector<Mat4f> inverseBindMatrices;
};

struct Texture{
	char name[STRING_SIZE];
	unsigned int ID;
};

struct Shader{
	char name[STRING_SIZE];
	unsigned int ID;
};

/*
struct TextureAtlas{
	std::vector<SmallString> names;
	std::vector<Vec4f> textureCoordinates;
	Texture texture;
};
*/

struct TextureBuffer{
	unsigned int VBO;
	unsigned int TB;
	unsigned int n_elements;
	unsigned int elementSize;
};

/*
typedef struct VertexMesh{
	Vec3f *vertices;
	int length;
}VertexMesh;
*/

struct PointMesh{
	Vec3f *points;
	int n_points;
	char name[STRING_SIZE];
};
