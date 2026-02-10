#include "AudioManager.h"
#include <iostream>

AudioManager::~AudioManager() {
    for (auto& pair : m_sounds) {
        Mix_FreeChunk(pair.second);
    }
    m_sounds.clear();
    
    if (m_currentMusic) {
        Mix_FreeMusic(m_currentMusic);
    }
    
    Mix_CloseAudio();
    Mix_Quit();
}

bool AudioManager::Initialize() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer initialization failed: " << Mix_GetError() << std::endl;
        return false;
    }
    
    m_currentMusic = nullptr;
    return true;
}

void AudioManager::PlayMusic(const std::string& filepath, int loops) {
    if (m_currentMusic) {
        Mix_FreeMusic(m_currentMusic);
    }
    
    m_currentMusic = Mix_LoadMUS(filepath.c_str());
    if (!m_currentMusic) {
        std::cerr << "Failed to load music " << filepath << ": " << Mix_GetError() << std::endl;
        return;
    }
    
    Mix_PlayMusic(m_currentMusic, loops);
}

void AudioManager::PlaySound(const std::string& filepath) {
    Mix_Chunk* sound = nullptr;
    
    auto it = m_sounds.find(filepath);
    if (it != m_sounds.end()) {
        sound = it->second;
    } else {
        sound = Mix_LoadWAV(filepath.c_str());
        if (!sound) {
            std::cerr << "Failed to load sound " << filepath << ": " << Mix_GetError() << std::endl;
            return;
        }
        m_sounds[filepath] = sound;
    }
    
    Mix_PlayChannel(-1, sound, 0);
}

void AudioManager::StopMusic() {
    Mix_HaltMusic();
}

void AudioManager::SetMusicVolume(int volume) {
    Mix_VolumeMusic(volume);
}

void AudioManager::SetSoundVolume(int volume) {
    Mix_Volume(-1, volume);
}
