#pragma once
#include <SDL3/SDL.h>
#include "Component.h"
#include "../Vec2.h"

class CollisionComponent : public Component {
 public:
  using Component::Component;

  Vec2 GetSize() const { return {Width, Height}; }
  Vec2 GetCenter() const {
    const auto& [x, y, w, h]{GetBounds()};
    return {
      x + w / 2,
      y + h / 2,
    };
  }

  void SetOffset(const Vec2& NewOffset);
  void SetSize(float NewWidth, float NewHeight);

  const SDL_FRect& GetBounds() const;

  bool IsCollidingWith(
    const CollisionComponent& Other
  ) const;

  bool GetCollisionRectangle(
    const CollisionComponent& Other,
    SDL_FRect* OutIntersection
  ) const;

  void Tick(float DeltaTime) override;
  void Initialize() override;
  void DrawDebugHelpers(SDL_Surface*) override;

 private:
  Vec2 Offset{0.0, 0.0};
  float Width{1.0};
  float Height{1.0};

  SDL_FRect Bounds{0.0, 0.0, 0.0, 0.0};
};
