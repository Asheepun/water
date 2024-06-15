struct Particle{
	Vec2f pos;
	Vec2f velocity;
};

//struct CollisionGridCell{
	//int indices[4];
	//int index;
	//int n_indices;
//};

int gridIndex(Vec2f pos, int bufferWidth){
	return (int)pos.y * bufferWidth + (int)pos.x;
}

int gridIndex(int x, int y, int bufferWidth){
	return (int)y * bufferWidth + (int)x;
}

int gridIndex_buffered(Vec2f pos, int bufferWidth){
	return ((int)pos.y + 1) * (bufferWidth + 2) + ((int)pos.x + 1);
}

const int WIDTH = (1920 / 2) / 2;
const int HEIGHT = (1080 / 2) / 2;

const int GRID_WIDTH = WIDTH / 4;
const int GRID_HEIGHT = HEIGHT / 4;

const float PARTICLE_GRAVITY = 0.06;

//const int N_INDICES_PER_COLLISION_GRID_CELL = 4;

Renderer2D_Renderer renderer;

Texture screenTexture;
uint8_t *screenPixels;

Array<Particle> particles;
bool *particleOccupiedGrid;

Vec2f *edges;
Vec2f *edgesPrevious;
Vec2f *edgeMasses;

//uint16_t collisionGrid[WIDTH * HEIGHT][N_INDICES_PER_COLLISION_GRID_CELL];
//Vec2f collisionGrid[WIDTH * HEIGHT][N_INDICES_PER_COLLISION_GRID_CELL];
Vec2f collisionGrid[WIDTH * HEIGHT][4];
uint8_t collisionGridCounts[WIDTH * HEIGHT];

Shader waterShader;

void Engine_start(){

	Renderer2D_init(&renderer, WIDTH, HEIGHT, Engine_assetManager_p);

	Shader_init(&waterShader, "water", "shaders/opengl/renderer2d/color-vertex-shader.glsl",  "shaders/opengl/water-fragment-shader.glsl", Engine_assetManager_p);

	screenPixels = (uint8_t *)malloc(WIDTH * HEIGHT * 4 * sizeof(uint8_t));
	memset(screenPixels, 0, WIDTH * HEIGHT * 4 * sizeof(uint8_t));

	particles.init();

	for(int x = 0; x < 400; x++){
		for(int y = 0; y < 250; y++){

			Particle particle;
			particle.pos = getVec2f(10.0 + x, 10.0 + y);
			particle.velocity = getVec2f(1.0);
			particles.push(particle);
		
		}
	}

	particleOccupiedGrid = (bool *)malloc((GRID_WIDTH) * (GRID_HEIGHT) * sizeof(bool));

	edges = (Vec2f *)malloc((GRID_WIDTH + 1) * (GRID_HEIGHT + 1) * sizeof(Vec2f));
	memset(edges, 0, (GRID_WIDTH + 1) * (GRID_HEIGHT + 1) * sizeof(Vec2f));

	edgesPrevious = (Vec2f *)malloc((GRID_WIDTH + 1) * (GRID_HEIGHT + 1) * sizeof(Vec2f));
	memset(edgesPrevious, 0, (GRID_WIDTH + 1) * (GRID_HEIGHT + 1) * sizeof(Vec2f));

	edgeMasses = (Vec2f *)malloc((GRID_WIDTH + 1) * (GRID_HEIGHT + 1) * sizeof(Vec2f));
	memset(edgeMasses, 0, (GRID_WIDTH + 1) * (GRID_HEIGHT + 1) * sizeof(Vec2f));
	
}

void Engine_initDrawing(){

}

void Engine_update(){

	if(Engine_keys[ENGINE_KEY_Q].downed){
		Engine_quit();
	}
	if(Engine_keys[ENGINE_KEY_F].downed){
		Engine_toggleFullscreen();
	}

	long long st = getTime_us();

	memset(particleOccupiedGrid, 0, GRID_WIDTH * GRID_HEIGHT * sizeof(bool));

	memset(edges, 0, (GRID_WIDTH + 1) * (GRID_HEIGHT + 1) * sizeof(Vec2f));
	memset(edgeMasses, 0, (GRID_WIDTH + 1) * (GRID_HEIGHT + 1) * sizeof(Vec2f));

	float h = (float)WIDTH / (float)GRID_WIDTH;

	long long ptgst = getTime_us();

	for(int i = 0; i < particles.length; i++){

		Particle *particle_p = &particles[i];

		particle_p->velocity.y += PARTICLE_GRAVITY;

		int x = particle_p->pos.x / h;
		int y = particle_p->pos.y / h;

		float dx = particle_p->pos.x - (float)x * h;
		float dy = particle_p->pos.y - (float)y * h;

		float weightWest  = square(dx) + square(0.5 * h - fabs(0.5 * h - dy));
		float weightNorth = square(dy) + square(0.5 * h - fabs(0.5 * h - dx));
		float weightEast  = square(h - dx) + square(0.5 * h - fabs(0.5 * h - dy));
		float weightSouth = square(h - dy) + square(0.5 * h - fabs(0.5 * h - dx));

		edges[gridIndex(x + 0, y + 0, GRID_WIDTH)].x += particle_p->velocity.x * weightWest;
		edges[gridIndex(x + 0, y + 0, GRID_WIDTH)].y += particle_p->velocity.y * weightNorth;
		edges[gridIndex(x + 1, y + 0, GRID_WIDTH)].x += particle_p->velocity.x * weightEast;
		edges[gridIndex(x + 0, y + 1, GRID_WIDTH)].y += particle_p->velocity.y * weightSouth;

		edgeMasses[gridIndex(x + 0, y + 0, GRID_WIDTH)].x += weightWest;
		edgeMasses[gridIndex(x + 0, y + 0, GRID_WIDTH)].y += weightNorth;
		edgeMasses[gridIndex(x + 1, y + 0, GRID_WIDTH)].x += weightEast;
		edgeMasses[gridIndex(x + 0, y + 1, GRID_WIDTH)].y += weightSouth;

		particleOccupiedGrid[gridIndex(x, y, GRID_WIDTH)] = true;

	}

	long long ptget = getTime_us();

	long long gst = getTime_us();

	for(int i = 0; i < (GRID_WIDTH + 1) * (GRID_HEIGHT + 1); i++){
		edges[i].x /= fmax(edgeMasses[i].x, 0.0001);
		edges[i].y /= fmax(edgeMasses[i].y, 0.0001);
	}

	float overrelaxation = 1.9;
	float stiffness = 0.0002;
	float restDensity = h * h;

	for(int iterations = 0; iterations < 2; iterations++){
		for(int y = 1; y < GRID_HEIGHT - 1; y++){
			for(int x = 1; x < GRID_WIDTH - 1; x++){
				
				float div = + edges[gridIndex(x + 0, y + 0, GRID_WIDTH)].x
							+ edges[gridIndex(x + 0, y + 0, GRID_WIDTH)].y
							- edges[gridIndex(x + 1, y + 0, GRID_WIDTH)].x
							- edges[gridIndex(x + 0, y + 1, GRID_WIDTH)].y;

				div *= overrelaxation;

				float density = edgeMasses[gridIndex(x, y, GRID_WIDTH)].x + edgeMasses[gridIndex(x, y, GRID_WIDTH)].y + edgeMasses[gridIndex(x + 1, y, GRID_WIDTH)].x + edgeMasses[gridIndex(x, y + 1, GRID_WIDTH)].y;
				div += stiffness * (density - restDensity);

				div *= (float)particleOccupiedGrid[gridIndex(x, y, GRID_WIDTH)];
				
				edges[gridIndex(x + 0, y + 0, GRID_WIDTH)].x -= div / 4.0;
				edges[gridIndex(x + 0, y + 0, GRID_WIDTH)].y -= div / 4.0;
				edges[gridIndex(x + 1, y + 0, GRID_WIDTH)].x += div / 4.0;
				edges[gridIndex(x + 0, y + 1, GRID_WIDTH)].y += div / 4.0;

			}
		}
	}

	long long get = getTime_us();

	memset(collisionGrid, 0, WIDTH * HEIGHT * sizeof(Vec2f) * 4);
	memset(collisionGridCounts, 0, WIDTH * HEIGHT * sizeof(uint8_t));

	long long gtpst = getTime_us();

	for(int i = 0; i < particles.length; i++){

		Particle *particle_p = &particles[i];

		int x = particle_p->pos.x / h;
		int y = particle_p->pos.y / h;

		float dx = particle_p->pos.x - (float)x * h;
		float dy = particle_p->pos.y - (float)y * h;

		float weightWest  = square(dx) + square(0.5 * h - fabs(0.5 * h - dy));
		float weightNorth = square(dy) + square(0.5 * h - fabs(0.5 * h - dx));
		float weightEast  = square(h - dx) + square(0.5 * h - fabs(0.5 * h - dy));
		float weightSouth = square(h - dy) + square(0.5 * h - fabs(0.5 * h - dx));

		float picWest  = edges[gridIndex(x + 0, y + 0, GRID_WIDTH)].x;
		float picNorth = edges[gridIndex(x + 0, y + 0, GRID_WIDTH)].y;
		float picEast  = edges[gridIndex(x + 1, y + 0, GRID_WIDTH)].x;
		float picSouth = edges[gridIndex(x + 0, y + 1, GRID_WIDTH)].y;

		Vec2f gridVelocity = getVec2f(
			(picWest * weightWest + picEast * weightEast) / fmax(weightWest + weightEast, 0.0001),
			(picNorth * weightNorth + picSouth * weightSouth) / fmax(weightNorth + weightSouth, 0.0001)
		);

		particle_p->velocity = gridVelocity;

		particle_p->pos += particle_p->velocity;

		float GROUND_RESISTANCE = 0.7;

		if(particle_p->pos.x < 10){
			particle_p->pos.x = 10;
			particle_p->velocity.x *= -GROUND_RESISTANCE;
		}
		if(particle_p->pos.y < 10){
			particle_p->pos.y = 10;
			particle_p->velocity.y *= -GROUND_RESISTANCE;
		}
		if(particle_p->pos.x > WIDTH - 10){
			particle_p->pos.x = WIDTH - 10;
			particle_p->velocity.x *= -GROUND_RESISTANCE;
		}
		if(particle_p->pos.y > HEIGHT - 10){
			particle_p->pos.y = HEIGHT - 10;
			particle_p->velocity.y *= -GROUND_RESISTANCE;
		}

		int collisionGridIndex = gridIndex(particle_p->pos, WIDTH);
		if(collisionGridCounts[collisionGridIndex] < 4){
			collisionGrid[collisionGridIndex][collisionGridCounts[collisionGridIndex]] = particle_p->pos;
			collisionGridCounts[collisionGridIndex]++;
		}
	
	}

	long long gtpet = getTime_us();

	long long cst = getTime_us();

	for(int i = 0; i < particles.length; i++){

		Particle *particle_p = &particles[i];
		
		int collisionGridIndex = gridIndex(particle_p->pos.x, particle_p->pos.y, WIDTH);

		for(int j = 0; j < 4; j++){
			Vec2f diff = particle_p->pos - collisionGrid[collisionGridIndex][j];
			particle_p->pos += diff * (float)(dot(diff, diff) < 1.0);
		}

	}

	long long cet = getTime_us();

	long long et = getTime_us();

	print("---");
	printf("particle to grid time: %lli us\n", ptget - ptgst);
	printf("grid fix time: %lli us\n", get - gst);
	printf("grid to particle time: %lli us\n", gtpet - gtpst);
	printf("collision time: %lli us\n", cet - cst);
	printf("total time: %lli us\n", et - st);

}

void Engine_draw(){

	memset(screenPixels, 0, WIDTH * HEIGHT * 4 * sizeof(uint8_t));

	for(int i = 0; i < particles.length; i++){
		int index = gridIndex(particles[i].pos, WIDTH);
		screenPixels[index * 4 + 0] = 0;
		screenPixels[index * 4 + 1] = 0;
		screenPixels[index * 4 + 2] = 255;
		screenPixels[index * 4 + 3] = 255;
	}

	Texture_free(&screenTexture);

	Texture_init(&screenTexture, "screen", screenPixels, WIDTH, HEIGHT);

	//draw background
	Renderer2D_setShader(&renderer, &renderer.colorShader);

	Renderer2D_setColor(&renderer, getVec4f(0.0, 0.0, 0.0, 1.0));

	Renderer2D_setAlpha(&renderer, 1.0);

	Renderer2D_drawRectangle(&renderer, 0, 0, Engine_clientWidth, Engine_clientHeight);

	//draw particles
	Renderer2D_setShader(&renderer, &waterShader);

	Renderer2D_setTexture(&renderer, &screenTexture);

	GL3D_uniformVec2f(waterShader.ID, "textureScale", getVec2f(1.0 / (float)WIDTH, 1.0 / (float)HEIGHT) * 1.5);

	Renderer2D_drawRectangle(&renderer, 0, 0, Engine_clientWidth, Engine_clientHeight);

}

void Engine_finish(){

}
