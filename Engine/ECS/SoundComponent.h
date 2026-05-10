#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <SDL3/SDL.h>
#include "Component.h"
#include "Config.h"
#include "Engine/SoundAssetManager.h"

class SoundComponent : public Component {
  public:
    using Component::Component;
    SoundComponent(Entity* Owner, const std::string &FilePath);

    void Play() {
      if (!SDL_PutAudioStreamData(
          waveStream,
          AudioStruct.wav_data,
          AudioStruct.wav_len
        )) {
          std::cerr << "Pushing audio failed "
            << SDL_GetError() << '\n';
      } 

      if (!SDL_FlushAudioStream(waveStream)) {
        std::cerr << "Flushing audio failed "
          << SDL_GetError() << '\n';
      }
    }

    bool IsPlaying() const {
      return SDL_GetAudioStreamQueued(waveStream) > 0;
    }

    ~SoundComponent() {
      SDL_DestroyAudioStream(waveStream);
    }

  private:
    SDL_AudioStream *waveStream{nullptr};
    std::string SoundFilePath;
    AudioCacheStruct AudioStruct{};
};
