#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

void AudioFX::play(int loops/*==0*/)
{//loop -1 == loop 4ever, 0== play one time, 1== play two times...

	if (Mix_PlayChannel(-1, m_chunk, loops) == -1) {
		LOG("Mix_PlayChanel! SDL_Error: %s\n", SDL_GetError());
	}
}
bool AudioMusic::play(int loops/*==-1*/, float fade_time)
{//loop -1 == loop 4ever, 0== play 0 times, 1== play 1 times, 2== play 2 times...

	Mix_FadeInMusic(m_music, -1, (fade_time * 1000.0f));

	LOG("Successfully playing");
	return true;
}

void AudioMusic::pause()
{
	Mix_PauseMusic();
}

void AudioMusic::stop()
{
	Mix_HaltMusic();
}

void AudioMusic::resume()
{
	Mix_ResumeMusic();
}

//------------------------------------------ Audio engine
ModuleAudioEngine::ModuleAudioEngine(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleAudioEngine::~ModuleAudioEngine()
{
}
//initialice audio
bool ModuleAudioEngine::Init()
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// load support for the OGG format
	int flags = MIX_INIT_OGG;	// can support MIX_INIT_FAC, MIX_INIT_MP3, MIX_INIT_OGG, MIX_INIT_MOD,
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = true;
		m_isInitialized = true;
	}

	return ret;

}

//destroy all the elements
bool ModuleAudioEngine::CleanUp()
{
	if (m_isInitialized) {
		m_isInitialized = false;
		Mix_CloseAudio();
		Mix_Quit();
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
	}

	return true;

}

AudioFX ModuleAudioEngine::LoadAudioFX(const std::string & filePath)
{
	//look to the cachhe to found the audio
	auto it = m_effectMap.find(filePath);

	AudioFX ret;

	if (it == m_effectMap.end()) {
		//failed to find -> load
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());

		if (chunk == nullptr) {
			LOG("Mix_LoadWAV! SDL_Error: %s\n", SDL_GetError());
		}

		ret.m_chunk = chunk;
		m_effectMap[filePath] == chunk;

	}
	else {
		//is on cache
		ret.m_chunk = it->second;

	}


	return ret;
}

AudioMusic ModuleAudioEngine::LoadAudioMusic(const std::string & filePath)
{

	//look to the cachhe to found the audio
	auto it = m_musicMap.find(filePath);

	AudioMusic ret;

	if (it == m_musicMap.end()) {
		//failed to find -> load
		Mix_Music* MixMusic = Mix_LoadMUS(filePath.c_str());

		if (MixMusic == nullptr) {
			LOG("Mix_LoadWAV! SDL_Error: %s\n", SDL_GetError());
		}

		ret.m_music = MixMusic;
		m_musicMap[filePath] == MixMusic;

	}
	else {
		//is on cache
		ret.m_music = it->second;
	
	}

	return ret;
}
//------------------------------------------

