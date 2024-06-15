#version 450 core

const int WIDTH = 128 * 4;
const int HEIGHT = 128 * 4;

const int WORK_GROUP_SIZE = 16;

layout (local_size_x = WORK_GROUP_SIZE, local_size_y = WORK_GROUP_SIZE, local_size_z = 1) in;

layout(std140, binding = 0) buffer Edges{
	vec4 elements[];
}edges;

void main(){

	uint bufferIndex = gl_GlobalInvocationID.y * WIDTH + gl_GlobalInvocationID.x;

	edges.elements[bufferIndex] = 2 * edges.elements[bufferIndex];

}
