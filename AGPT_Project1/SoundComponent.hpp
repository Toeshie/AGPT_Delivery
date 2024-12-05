#pragma once
#include "Component.hpp"
#include <SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <memory>

class SoundComponent : public Component {
public:
    SoundComponent() {
        // Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        }
    }

    ~SoundComponent() {
        // Clean up sounds
        for (auto& [name, sound] : soundEffects) {
            Mix_FreeChunk(sound);
        }
        for (auto& [name, music] : backgroundMusic) {
            Mix_FreeMusic(music);
        }
        soundEffects.clear();
        backgroundMusic.clear();
    }

    // Load sound effects (for one-shots)
    bool LoadSoundEffect(const std::string& name, const std::string& path) {
        Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
        if (!sound) {
            SDL_Log("Failed to load sound effect: %s\n", Mix_GetError());
            return false;
        }
        soundEffects[name] = sound;
        return true;
    }

    // Load background music
    bool LoadMusic(const std::string& name, const std::string& path) {
        Mix_Music* music = Mix_LoadMUS(path.c_str());
        if (!music) {
            SDL_Log("Failed to load music: %s\n", Mix_GetError());
            return false;
        }
        backgroundMusic[name] = music;
        return true;
    }

    // Play a sound effect once
    void PlaySound(const std::string& name, int volume = 128) {
        auto it = soundEffects.find(name);
        if (it != soundEffects.end()) {
            Mix_VolumeChunk(it->second, volume);
            Mix_PlayChannel(-1, it->second, 0);
        }
    }

    // Play background music
    void PlayMusic(const std::string& name, int loops = -1, int volume = 128) {
        auto it = backgroundMusic.find(name);
        if (it != backgroundMusic.end()) {
            Mix_VolumeMusic(volume);
            Mix_PlayMusic(it->second, loops); // -1 for infinite loops
        }
    }

    // Control methods
    void StopMusic() {
        Mix_HaltMusic();
    }

    void PauseMusic() {
        Mix_PauseMusic();
    }

    void ResumeMusic() {
        Mix_ResumeMusic();
    }

    void SetMusicVolume(int volume) {
        Mix_VolumeMusic(volume);  // 0-128
    }

    std::string GetName() const override { return "SoundComponent"; }

private:
    std::unordered_map<std::string, Mix_Chunk*> soundEffects;
    std::unordered_map<std::string, Mix_Music*> backgroundMusic;
};