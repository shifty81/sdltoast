#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>

class AudioManager {
public:
    AudioManager() = default;
    ~AudioManager();

    bool Initialize();
    
    void PlayMusic(const std::string& filepath, int loops = -1);
    void PlaySound(const std::string& filepath);
    void StopMusic();
    
    void SetMusicVolume(int volume); // 0-128
    void SetSoundVolume(int volume); // 0-128

private:
    Mix_Music* m_currentMusic;
    std::unordered_map<std::string, Mix_Chunk*> m_sounds;
};

#endif // AUDIOMANAGER_H
