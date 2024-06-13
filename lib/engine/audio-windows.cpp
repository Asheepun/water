#ifdef _WIN32

#include "engine/audio.h"

#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_DECODING
#define MA_NO_ENCODING
#define MA_NO_GENERATION
#define MA_ENABLE_WASAPI 

#include <miniaudio/miniaudio.h>

ma_result result;
ma_device_config deviceConfig;
ma_device device;

float soundTypeVolumes[N_AUDIO_SOUND_TYPES];

void data_callback(ma_device* device_p, void* voidOutput_p, const void* input_p, ma_uint32 frameCount){

	float MASTER_VOLUME = 0.5;

	int16_t *output_p = (int16_t *)voidOutput_p;


	for(int i = 0; i < sounds.size(); i++){

		Sound *sound_p = &sounds[i];

		for(int j = 0; j < frameCount; j++){

			if(sound_p->n_playedSamples >= sound_p->soundData_p->n_samples){
				if(sound_p->loop){
					sound_p->n_playedSamples = 0;
				}else{
					break;
				}
			}

			int16_t value = (int16_t)(
				MASTER_VOLUME *
				soundTypeVolumes[sound_p->type] * 
				sound_p->volume *
				(float)sound_p->soundData_p->data[sound_p->n_playedSamples * sound_p->soundData_p->n_channels]
			);

			output_p[j * 2 + 0] += value;
			output_p[j * 2 + 1] += value;

			sound_p->n_playedSamples++;
		
		}

		if(!sound_p->loop
		&& sound_p->n_playedSamples >= sound_p->soundData_p->n_samples){
			sounds.erase(sounds.begin() + i);
			i--;
			continue;
		}
	
	}

}

void Audio_init(){

	for(int i = 0; i < N_AUDIO_SOUND_TYPES; i++){
		soundTypeVolumes[i] = 0.5;
	}

	deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = ma_format_s16;
    //deviceConfig.playback.format   = ma_format_f32;
    deviceConfig.playback.channels = 2;
    deviceConfig.sampleRate        = AUDIO_SAMPLE_RATE;
    deviceConfig.dataCallback      = data_callback;
	
	if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
		printf("Could not initialize miniaudio device!\n");
    }

	ma_device_start(&device);

}

#endif
