#ifdef __linux__

//#include "engine/audio.h"
//#include "engine/array.h"

#include <pulse/simple.h>
#include <pulse/error.h>

//#include "stdio.h"
//#include "math.h"
//#include "pthread.h"

pa_simple *s;
pa_sample_spec ss;

void *audioDataLoop(void *);

float frequency = 1 / 10.0;

float soundTypeVolumes[N_AUDIO_SOUND_TYPES];

void Audio_init(){

	for(int i = 0; i < N_AUDIO_SOUND_TYPES; i++){
		soundTypeVolumes[i] = 0.5;
	}

	ss.format = PA_SAMPLE_S16NE;
	ss.channels = 1;
	ss.rate = AUDIO_SAMPLE_RATE;

	s = pa_simple_new(
		NULL,               // Use the default server.
		"Game",           // Our application's name.
		PA_STREAM_PLAYBACK,
		NULL,               // Use the default device.
		"Audio",            // Description of our stream.
		&ss,                // Our sample format.
		NULL,               // Use default channel map
		NULL,               // Use default buffering attributes.
		NULL               // Ignore error code.
	);

	pthread_t audioDataLoopThread;
	pthread_create(&audioDataLoopThread, NULL, audioDataLoop, NULL);

}

void *audioDataLoop(void *args){

	float MASTER_VOLUME = 0.1;
	MASTER_VOLUME = 0.5;
	//MASTER_VOLUME = 0.0;

	int BUFFER_LENGTH = 64;

	int counter = 0;

	//int16_t bufferA[BUFFER_LENGTH];
	int16_t bufferB[BUFFER_LENGTH];

	//int16_t *frontBuffer = bufferA;
	int16_t *backBuffer = bufferB;

	while(true){

		//fill backbuffer
		memset(backBuffer, 0, BUFFER_LENGTH * sizeof(int16_t));

		for(int i = 0; i < sounds.length; i++){

			Sound *sound_p = &sounds[i];

			for(int j = 0; j < BUFFER_LENGTH; j++){

				if(sound_p->n_playedSamples >= sound_p->soundData_p->n_samples){
					if(sound_p->loop){
						sound_p->n_playedSamples = 0;
					}else{
						break;
					}
				}

				backBuffer[j] += (int16_t)(
					MASTER_VOLUME *
					soundTypeVolumes[sound_p->type] * 
					sound_p->volume *
					(float)sound_p->soundData_p->data[sound_p->n_playedSamples * sound_p->soundData_p->n_channels]
				);

				sound_p->n_playedSamples++;
			
			}

			if(!sound_p->loop
			&& sound_p->n_playedSamples >= sound_p->soundData_p->n_samples){
				sounds.remove(i);
				i--;
				continue;
			}
		
		}

		//write front buffer to sound card
		int error;
		pa_simple_write(s, backBuffer, BUFFER_LENGTH * sizeof(int16_t), &error);

		//switch buffers
		//int16_t *tmpPointer = backBuffer;
		//backBuffer = frontBuffer;
		//frontBuffer = tmpPointer;
		
	}

}

void Audio_setFrequency(float x){
	frequency = x;
}

void Audio_pausePlayback(){
	//do nothing
}

void Audio_resumePlayback(){
	//do nothing
}

#endif
