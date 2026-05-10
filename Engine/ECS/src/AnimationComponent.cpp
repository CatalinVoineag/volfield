#include <SDL3/SDL.h>
#include <iostream>
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/AnimationComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/Utilities.h"

AnimationComponent::AnimationComponent(
  Entity* Owner,
  const std::string& FilePath,
  int SpriteFrames,
  int SpriteWidth,
  int SpriteHeight,
  Vec2 Offset
  ) :
  Component(Owner),
  AnimationFilePath(FilePath),
  SpriteFrames{SpriteFrames},
  SpriteWidth{SpriteWidth},
  SpriteHeight{SpriteHeight},
  Offset{Offset} {

  AnimationSurface = GetAssetManager()
    .LoadSurface(AnimationFilePath);
}

void AnimationComponent::Render(SDL_Surface* Surface, float DeltaTime) {
  if (!AnimationSurface || !Play) return;

  DeltaTimeCounter += DeltaTime;

  if (DeltaTimeCounter >= AnimationTime) {
    DeltaTimeCounter = 0;
    if (SpriteSheetIndex == SpriteFrames) {
      Play = false;
    } else {
      SpriteSheetIndex++;
    }
  }

    auto [TargetX, TargetY]{GetOwnerPosition() + Offset};

    SDL_Rect rect {SpriteSheetIndex * SpriteWidth, 0, SpriteWidth, SpriteHeight};
    SDL_Rect DestRect {
      static_cast<int>(round(TargetX)),
      static_cast<int>(round(TargetY)),
      SpriteWidth,
      SpriteHeight
    };

    if (!SDL_BlitSurfaceScaled(
          AnimationSurface.get(),
          &rect,
          Surface,
          &DestRect,
          SDL_SCALEMODE_LINEAR
          )) {
      std::cerr << "Error: Blit failed: "
        << SDL_GetError() << '\n';
    }
}
