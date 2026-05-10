#include <SDL3/SDL.h>
#include <iostream>
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/SoundComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/Utilities.h"

SoundComponent::SoundComponent(
  Entity* Owner, const std::string& FilePath
  ) : Component(Owner), SoundFilePath(FilePath) {
  AudioStruct = GetSoundAssetManager().LoadAudio(SoundFilePath);
  waveStream = SDL_CreateAudioStream(&AudioStruct.wave_spec, nullptr);

  if (!waveStream) {
    std::cerr << "Wave stream not initialized: "
              << SDL_GetError() << '\n';
  }

  if (!SDL_BindAudioStream(GetSoundAssetManager().DeviceID, waveStream)) {
    std::cerr << "Binding audio failed: "
              << SDL_GetError() << '\n';
  }
}

