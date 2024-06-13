#version 450 core

const int WORK_GROUP_SIZE = 128;

layout (local_size_x = WORK_GROUP_SIZE, local_size_y = 1, local_size_z = 1) in;

layout(std140, binding = 0) buffer InputBuffer{
	ivec4 numbers[];
}inputBuffer;

layout(std140, binding = 1) buffer OutputBuffer{
	ivec4 numbers[];
}ouputBuffer;

void main(){

	uint bufferIndex = gl_GlobalInvocationID.x;

	ouputBuffer.numbers[bufferIndex] += inputBuffer.numbers[bufferIndex];

}
