struct Renderer2D_Renderer{
	//int width;
	//int height;
	Vec2f offset;
	Vec2f scale;
	float borderRadius;

	Model quadModel;

	unsigned int rectangleVBO;
	unsigned int rectangleVAO;

	Shader colorShader;
	Shader textureShader;
	Shader textureColorShader;
	Shader iconShader;

	Shader currentShader;

	bool drawAroundCenter;

#ifdef __APPLE__
	MTL::RenderCommandEncoder *renderCommandEncoder_p;
#endif

};
