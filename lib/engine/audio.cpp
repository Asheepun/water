#include "engine/audio.h"
//#include "engine/files.h"
//#include "engine/geometry.h"
//#include "engine/engine.h"
//#include "engine/assetManager.h"

#include "stb/stb_vorbis.h"

Array<Sound> sounds;

Array<SoundData> soundDatas;
Array<SoundConfig> soundConfigs;

int __currentSoundID__ = 0;

void SoundData_initFromFile(SoundData *soundData_p, const char *path, const char *name, AssetManager *assetManager_p){

	String_set(soundData_p->name, name, STRING_SIZE);

    long int fileSize;
    char *fileData = getAssetData_mustFree(path, &fileSize, assetManager_p);

	int sampleRate;
    soundData_p->n_samples = stb_vorbis_decode_memory((unsigned char *)fileData, fileSize, &soundData_p->n_channels, &sampleRate, &soundData_p->data);

	if(soundData_p->n_samples < 0){
		printf("Could not load audio file %s\n", path);
	}

    free(fileData);

}

void Audio_loadSoundConfig(const char *path){

	soundConfigs.clear();

	int n_lines;
	String *fileLines = getAssetLines_mustFree(path, &n_lines, Engine_assetManager_p);

	for(int i = 0; i < n_lines; i++){

		if(fileLines[i] == ":Sound"){

			char *ptr;

			SoundConfig config;

			String_set(config.name, fileLines[i + 1], SMALL_STRING_SIZE);

			config.volume = strtof(fileLines[i + 2], &ptr);

			config.n_variations = strtol(fileLines[i + 3], &ptr, 10);

			soundConfigs.push(config);

		}

	}

	free(fileLines);

}

int Audio_playSoundData(SoundData *soundData_p, float volume, bool loop, enum SoundType type){

	Sound sound;
	sound.ID = __currentSoundID__;
	__currentSoundID__++;

	sound.soundData_p = soundData_p;
	sound.n_playedSamples = 0;
	sound.volume = volume;
	sound.loop = loop;
	sound.type = type;

	sounds.push(sound);

	return sound.ID;

}

int Audio_playSound(const char *name, float volume, bool loop, enum SoundType type){

	for(int i = 0; i < soundDatas.length; i++){
		if(strcmp(soundDatas[i].name, name) == 0){
			return Audio_playSoundData(&soundDatas[i], volume, loop, type);
		}
	}

	printf("Could not find sound with name: %s\n", name);

	return -1;

}

int Audio_playSoundConfig(const char *name, float volume, bool loop, enum SoundType type){

	SoundConfig config;

	for(int i = 0; i < soundConfigs.length; i++){
		if(soundConfigs[i].name == name){
			config = soundConfigs[i];
			break;
		}
	}

	int variation = (int)(1.0 + getRandom() * (float)config.n_variations);

	String variationName;
	String_set(variationName, config.name, STRING_SIZE);
	String_append(variationName, "-");
	String_append_int(variationName, variation);

	return Audio_playSound(variationName, volume * config.volume, loop, type);

}

float Audio_getSoundConfigVolume(const char *name){

	SoundConfig config;

	for(int i = 0; i < soundConfigs.length; i++){
		if(soundConfigs[i].name == name){
			return soundConfigs[i].volume;
		}
	}

	return 0.0;

}
