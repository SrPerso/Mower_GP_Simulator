#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include <map> 


#define DEFAULT_MUSIC_FADE_TIME 2.0f

//------------------------
class AudioFX {
	friend class ModuleAudioEngine;
public:
	void play(int nLoops = 0);//default will play once

public:
	Mix_Chunk* m_chunk = nullptr;
};
//------------------------

//------------------------
class AudioMusic {
	friend class ModuleAudioEngine;
public:
	bool play(int nLoops = -1, float fade_time = DEFAULT_MUSIC_FADE_TIME);//default will play once
	static void pause();
	static void stop();
	static void resume();


private:
	Mix_Music* m_music = nullptr;
};
//------------------------

//------------------------
class ModuleAudioEngine : public Module {
public:
	ModuleAudioEngine(Application* app, bool start_enabled = true);
	~ModuleAudioEngine();

	bool Init();
	bool CleanUp();

	void setMusicVolume(int);

public:
	AudioFX LoadAudioFX(const std::string&filePath);
	AudioMusic LoadAudioMusic(const std::string&filePath);

private:

	std::map<std::string, Mix_Chunk*> m_effectMap;
	std::map<std::string, Mix_Music*> m_musicMap;

	bool m_isInitialized = false;
};
//------------------------


#endif // __ModuleAudio_H__