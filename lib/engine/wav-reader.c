#include "engine/wav-reader.h"

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "stdarg.h"
#include "stdint.h"
#include "stddef.h"

#define HEADER_READ_LENGTH 1000

#define FILE_SIZE_INDEX 4
#define FORMAT_TYPE_INDEX 20
#define NUMBER_OF_CHANNELS_INDEX 22
#define SAMPLE_RATE_INDEX 24
#define BITS_PER_SAMPLE_INDEX 34
#define DATA_SIZE_INDEX 40

#define BEXT_INDEX 36

#define SAMPLE_RATE 44100
#define NUMBER_OF_CHANNELS 2

//#define LOG_INFO_WAV_READER

void local_printf(const char *format, ...){

	va_list args;
    va_start(args, format);

#ifdef LOG_INFO_WAV_READER
	vprintf(format, args);
#endif

    va_end(args);

}

void lowPassFrequency(int16_t *, int16_t *, int, int, int, int);

int16_t *WavReader_getDataFromWavFile(char *path, int *numberOfPcmFrames_p){

	FILE *fileHandle = NULL;
	
	fileHandle = fopen(path, "r");

	if(fileHandle == NULL){

		printf("COULD NOT READ FILE: %s\n", path);
		
		return NULL;
	}

	local_printf("\nfile path: %s\n\n", path);

	//read header data
	int8_t fileHeader[HEADER_READ_LENGTH];
	memset(fileHeader, 0, HEADER_READ_LENGTH);

	fread(fileHeader, 1, HEADER_READ_LENGTH, fileHandle);

	local_printf("RAW HEADER\n");
	for(int i = 0; i < HEADER_READ_LENGTH; i++){
		//local_printf("%c", fileHeader[i]);
	}
	local_printf("\n\n");

	int32_t sampleRate;
	int16_t numberOfChannels;
	int16_t bitsPerSample;
	int32_t dataSize;

	memcpy(&numberOfChannels, fileHeader + NUMBER_OF_CHANNELS_INDEX, 2);
	memcpy(&sampleRate, fileHeader + SAMPLE_RATE_INDEX, 4);
	memcpy(&bitsPerSample, fileHeader + BITS_PER_SAMPLE_INDEX, 2);
	memcpy(&dataSize, fileHeader + DATA_SIZE_INDEX, 4);

	int audioDataIndex = DATA_SIZE_INDEX + 4;

	//check for BEXT CHUNK
	{
		char check[5];
		check[4] = *"\0";
		memcpy(check, fileHeader + BEXT_INDEX, 4);

		if(strcmp(check, "bext") == 0){

			local_printf("---\nFOUND BEXT!\n---\n");

			int dataSizeIndex;

			for(int i = 0; i < HEADER_READ_LENGTH; i++){

				char check[5];
				check[4] = *"\0";
				memcpy(check, fileHeader + i, 4);

				if(strcmp(check, "data") == 0){
					dataSizeIndex = i + 4;
					break;
				}

			}

			memcpy(&dataSize, fileHeader + dataSizeIndex, 4);
		
			audioDataIndex = dataSizeIndex + 4;

		}
	
	}

	local_printf("numberOfChannels: %i\n", numberOfChannels);
	local_printf("sampleRate: %i\n", sampleRate);
	local_printf("dataSize: %i\n", dataSize);
	local_printf("bitsPerSample: %i\n", bitsPerSample);

	int numberOfPcmFrames = dataSize / numberOfChannels / (bitsPerSample / 8);

	int time = numberOfPcmFrames / sampleRate;

	local_printf("\nnumberOfPcmFrames: %i\n", numberOfPcmFrames);
	local_printf("time: %i seconds\n", time);

	//read audio data

	local_printf("\nBEGAN PROCESSING DATA\n");

	int16_t *audioData = (int16_t *)malloc(sizeof(int16_t) * numberOfPcmFrames * numberOfChannels);

	memset(audioData, 0, sizeof(int16_t) * numberOfPcmFrames * numberOfChannels);

	fseek(fileHandle, audioDataIndex, SEEK_SET);

	fread(audioData, 1, dataSize, fileHandle);

	local_printf("\nFINNISHED PROCESSING DATA\n");

	fclose(fileHandle);

	*numberOfPcmFrames_p = numberOfPcmFrames;

	return audioData;

}
