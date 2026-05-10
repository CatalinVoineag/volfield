#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <optional>
#include <SDL3/SDL.h>
#include "Component.h"
#include "Config.h"
#include "../AssetManager.h"
#include "../Vec2.h"

class AnimationComponent : public Component {
  public:
    using Component::Component;
    AnimationComponent(
      Entity* Owner,
      const std::string &FilePath,
      int SpriteFrames,
      int SpriteWidth,
      int SpriteHeight,
      Vec2 Offset
    );

    void Render(SDL_Surface* Surface, float DeltaTime) override;

    int GetSurfaceWidth() const;
    int GetSurfaceHeight() const;
    float GetWidth() const;
    float GetHeight() const;
    bool Play{false};

  private:
  std::string AnimationFilePath;
  std::shared_ptr<SDL_Surface> AnimationSurface{nullptr};
  int SpriteSheetIndex{0};
  int SpriteFrames;
  int SpriteWidth{0};
  int SpriteHeight{0};
  Vec2 Offset{0, 0};
  float AnimationTime{.1f};
  float DeltaTimeCounter{0};
};
