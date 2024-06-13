#ifdef __APPLE__

#include "engine/audio.h"
#include "engine/strings.h"

#include "math.h"

#include <AudioToolbox/AudioToolbox.h>

int soundTime = 0;

float soundTypeVolumes[N_AUDIO_SOUND_TYPES];

void auCallback(void *inUserData, AudioQueueRef queue, AudioQueueBufferRef buffer){

	int framesToWrite = buffer->mAudioDataBytesCapacity / sizeof(int16_t);
	buffer->mAudioDataByteSize = framesToWrite * sizeof(int16_t);

	int16_t *outputData = (int16_t *)buffer->mAudioData;

	memset(outputData, 0, framesToWrite * sizeof(int16_t));

	int16_t stuff = outputData[0];

	float MASTER_VOLUME = 1.0;

	//for(int i = 0; i < framesToWrite; i++){
		//outputData[i] += int16_t(0.5 * 32768.0 * sin(soundTime / 10.0));
		//soundTime += 1.0;
	//}

	for(int i = 0; i < sounds.size(); i++){

		Sound *sound_p = &sounds[i];

		for(int j = 0; j < framesToWrite; j++){

			if(sound_p->n_playedSamples >= sound_p->soundData_p->n_samples){
				if(sound_p->loop){
					sound_p->n_playedSamples = 0;
				}else{
					break;
				}
			}

			outputData[j] += (int16_t)(
				MASTER_VOLUME *
				soundTypeVolumes[sound_p->type]	*
				sound_p->volume *
				(float)sound_p->soundData_p->data[sound_p->n_playedSamples * sound_p->soundData_p->n_channels]
			);

			sound_p->n_playedSamples++;
		
		}
		
		if(!sound_p->loop
		&& sound_p->n_playedSamples >= sound_p->soundData_p->n_samples){
			sounds.erase(sounds.begin() + i);
			i--;
			continue;
		}
	
	}

	AudioQueueEnqueueBuffer(queue, buffer, 0, 0);

}

void Audio_init(){

	for(int i = 0; i < N_AUDIO_SOUND_TYPES; i++){
		soundTypeVolumes[i] = 0.5;
	}

	AudioStreamBasicDescription auDescriptor = {};
	auDescriptor.mSampleRate = AUDIO_SAMPLE_RATE;
	auDescriptor.mFormatID = kAudioFormatLinearPCM;
	auDescriptor.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger
							  | kLinearPCMFormatFlagIsPacked;
	auDescriptor.mBytesPerPacket = sizeof(int16_t);
	auDescriptor.mFramesPerPacket = 1;
	auDescriptor.mBytesPerFrame = sizeof(int16_t);
	auDescriptor.mChannelsPerFrame = 1;
	auDescriptor.mBitsPerChannel = 8 * sizeof(int16_t);

	AudioQueueRef auQueue = 0;
	AudioQueueBufferRef auBuffer1 = 0;
	AudioQueueBufferRef auBuffer2 = 0;

	int soundState;

	OSStatus error;

	error = AudioQueueNewOutput(&auDescriptor, &auCallback, &soundState, 0, 0, 0, &auQueue);

	//uint32_t bufferSize = 512 * sizeof(int16_t);
	uint32_t bufferSize = 1024 * sizeof(int16_t);
	//uint32_t bufferSize = 2048 * sizeof(int16_t);

	error = AudioQueueAllocateBuffer(auQueue, bufferSize, &auBuffer1);
	error = AudioQueueAllocateBuffer(auQueue, bufferSize, &auBuffer2);

	auCallback(&soundState, auQueue, auBuffer1);
	auCallback(&soundState, auQueue, auBuffer2);

	error = AudioQueueEnqueueBuffer(auQueue, auBuffer1, 0, 0);
	error = AudioQueueEnqueueBuffer(auQueue, auBuffer2, 0, 0);

	error = AudioQueueStart(auQueue, NULL);

}

void Audio_pausePlayback(){
	//do nothing yet
}

void Audio_resumePlayback(){
	//do nothing yet
}

#endif
