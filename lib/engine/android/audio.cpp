//#include "engine/audio.h"

#include <aaudio/AAudio.h>

//#include "math.h"
//#include "string.h"
//#include "stdlib.h"

int elapsedFrames = 0;

float frequency = 2.0;

float soundTypeVolumes[N_AUDIO_SOUND_TYPES];

AAudioStream *stream;
bool initedStream = false;

aaudio_data_callback_result_t myCallback(AAudioStream *stream, void *userData, void *audioData, int32_t numFrames) {

    int16_t *output_p = (int16_t *)audioData;

    float MASTER_VOLUME = 0.5;

    memset(output_p, 0, numFrames * sizeof(int16_t));

    for(int i = 0; i < sounds.length; i++){

        Sound *sound_p = &sounds[i];

        for(int j = 0; j < numFrames; j++){

            if(sound_p->n_playedSamples >= sound_p->soundData_p->n_samples){
				if(sound_p->loop){
					sound_p->n_playedSamples = 0;
				}else{
					break;
				}
            }

            output_p[j] += (int16_t)(
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

/*
    for(int i = 0; i < numFrames; i++){

        output_p[i] = (65536.0 / 2.0) * sin(frequency * (float)elapsedFrames / 10.0);

        elapsedFrames++;

    }*/

    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

void Audio_init(){

	for(int i = 0; i < N_AUDIO_SOUND_TYPES; i++){
		soundTypeVolumes[i] = 0.5;
	}

    AAudioStreamBuilder *builder;
    aaudio_result_t result = AAudio_createStreamBuilder(&builder);

    AAudioStreamBuilder_setDeviceId(builder, AAUDIO_UNSPECIFIED);
    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
    AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_SHARED);
    AAudioStreamBuilder_setSampleRate(builder, AUDIO_SAMPLE_RATE);
    AAudioStreamBuilder_setChannelCount(builder, 1);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
    AAudioStreamBuilder_setBufferCapacityInFrames(builder, 512);

    AAudioStreamBuilder_setDataCallback(builder, myCallback, NULL);

    result = AAudioStreamBuilder_openStream(builder, &stream);

    result = AAudioStream_requestStart(stream);

	initedStream = true;

}

void Audio_pausePlayback(){
	if(initedStream){
		AAudioStream_requestPause(stream);
	}
}

void Audio_resumePlayback(){
	if(initedStream){
		AAudioStream_requestStart(stream);
	}
}
