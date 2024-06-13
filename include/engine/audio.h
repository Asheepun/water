enum SoundType{
	AUDIO_SOUND_TYPE_SFX,
	AUDIO_SOUND_TYPE_MUSIC,
	N_AUDIO_SOUND_TYPES,
};

struct SoundConfig{
	String name;
	float volume;
	int n_variations;
};

struct SoundData{
	int16_t *data;
	int n_channels;
	int n_samples;
	String name;
};

struct Sound{
	int ID;
	SoundData *soundData_p;
	float volume;
	int n_playedSamples;
	bool loop;
	enum SoundType type;
};

static const int AUDIO_SAMPLE_RATE = 44100;

extern Array<Sound> sounds;
extern Array<SoundData> soundDatas;
extern Array<SoundConfig> soundConfigs;

extern float soundTypeVolumes[N_AUDIO_SOUND_TYPES];

void Audio_init();

void Audio_pausePlayback();

void Audio_resumePlayback();

void SoundData_initFromFile(SoundData *, const char *, const char *, AssetManager *);

void Audio_loadSoundConfig(const char *);

int Audio_playSoundData(SoundData *, float, bool, enum SoundType);

int Audio_playSound(const char *, float, bool, enum SoundType);

int Audio_playSoundConfig(const char *, float, bool, enum SoundType);

float Audio_getSoundConfigVolume(const char *name);
