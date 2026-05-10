#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

struct AudioCacheStruct {
  SDL_AudioSpec wave_spec;
  Uint8 *wav_data = nullptr;
  Uint32 wav_len = 0;
};

using AudioCacheMap =
  std::unordered_map<std::string, AudioCacheStruct>;

class SoundAssetManager {
 public:
  SoundAssetManager() {
    DeviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (DeviceID == 0) {
      std::cerr << "Audio device failed to load: "
                << SDL_GetError() << '\n';
    }
  }
  SDL_AudioDeviceID DeviceID;

  AudioCacheStruct LoadAudio(const std::string& Path) {
    AudioCacheStruct AudioStruct{};

    if (AudioCache.contains(Path)) {
      // std::cout << "CACHE HIT: " << Path << '\n';
      return AudioCache.at(Path);
    }

    // std::cout << "CACHE MISS: " << Path << '\n';

    if (!SDL_LoadWAV(Path.c_str(), &AudioStruct.wave_spec, &AudioStruct.wav_data, &AudioStruct.wav_len)) {
      std::cerr << "Audio wave failed to load: "
                << Path << " Error: "
                << SDL_GetError() << '\n';
      return AudioStruct;
    }

    AudioCache[Path] = AudioStruct;
    return AudioStruct;
  }

  ~SoundAssetManager() {
    if (DeviceID) {
      SDL_CloseAudioDevice(DeviceID);
    }
    for (const auto& [Key, Value]: AudioCache) {
      if (Value.wav_data) {
        SDL_free(Value.wav_data);
      }
    }
  }

  // Prevent copying/moving
  SoundAssetManager(const SoundAssetManager&) = delete;
  SoundAssetManager& operator=(const SoundAssetManager&) = delete;
  SoundAssetManager(SoundAssetManager&&) = delete;
  SoundAssetManager& operator=(SoundAssetManager&&) = delete;

 private:
  AudioCacheMap AudioCache;
};
