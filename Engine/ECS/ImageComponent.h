#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL.h>
#include <optional>
#include "Component.h"
#include "../Vec2.h"

enum class ScalingMode {
  None, Fill, Contain, Cover
};

class ImageComponent : public Component {
 public:
  using Component::Component;
  ImageComponent(
    Entity* Owner,
    const std::string& FilePath
  );

  void Initialize() override;
  void Render(SDL_Surface* Surface, float DeltaTime) override;
  void DrawDebugHelpers(SDL_Surface*) override;
  bool LoadNewImage(const std::string& NewPath);

  int GetSurfaceWidth() const;
  int GetSurfaceHeight() const;
  void SetWidth(float NewWidth);
  void SetHeight(float NewHeight);
  void ResetWidth();
  void ResetHeight();
  float GetWidth() const;
  float GetHeight() const;
  void SetScalingMode(ScalingMode Mode);

  void SetOffset(const Vec2& NewOffset) {
    Offset = NewOffset;
  }

private:
  std::shared_ptr<SDL_Surface> ImageSurface{nullptr};
  std::string ImageFilePath;
  Vec2 Offset{0, 0};
  std::optional<float> Width{std::nullopt};
  std::optional<float> Height{std::nullopt};
  ScalingMode ScaleMode{ScalingMode::Fill};
};

