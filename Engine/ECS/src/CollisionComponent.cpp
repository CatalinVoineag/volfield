#include <iostream>
#include "Engine/ECS/CollisionComponent.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/Utilities.h"
#include "Engine/Scene.h"

void CollisionComponent::Tick(float DeltaTime) {
  Vec2 OwnerPos{GetOwnerPosition()};
  float OwnerScale{GetOwnerScale()};

  Bounds.x = OwnerPos.x + Offset.x;
  Bounds.y = OwnerPos.y + Offset.y;
  Bounds.w = Width * OwnerScale;
  Bounds.h = Height * OwnerScale;
}

void CollisionComponent::Initialize() {
  if (!GetOwner()->GetComponent<TransformComponent>()) {
    std::cerr << "Error: CollisionComponent "
      "requires TransformComponent on its Owner.\n";
    GetOwner()->RemoveComponent(this);
  }
}

void CollisionComponent::SetOffset(
  const Vec2& NewOffset
) {
  Offset = NewOffset;
}

void CollisionComponent::SetSize(
  float NewWidth, float NewHeight
) {
  if (NewWidth < 0 || NewHeight < 0) {
    std::cerr << "Warning: CollisionComponent "
      "width/height cannot be negative. "
      "Using absolute values.\n";
    Width = std::abs(NewWidth);
    Height = std::abs(NewHeight);
  } else {
    Width = NewWidth;
    Height = NewHeight;
  }
}

const SDL_FRect& CollisionComponent::GetBounds() const {
  return Bounds;
}

bool CollisionComponent::IsCollidingWith(
  const CollisionComponent& Other
) const {
  const SDL_FRect& A{GetBounds()};
  const SDL_FRect& B{Other.GetBounds()};

  return SDL_HasRectIntersectionFloat(&A, &B);
}

void CollisionComponent::DrawDebugHelpers(
  SDL_Surface* Surface
) {
  SDL_Rect ScreenBounds{
    Utilities::Round(Bounds)
  };

  const auto* Fmt{SDL_GetPixelFormatDetails(Surface->format)};
  Uint32 Color{SDL_MapRGB(
    Fmt, nullptr, 255, 255, 0
  )};

  if (!GetIsEnabled()) {
    Color = SDL_MapRGB(Fmt, nullptr, 255, 165, 0);
  };

  Utilities::DrawRectOutline(
    Surface,
    ScreenBounds,
    Color,
    1
  );

  auto [cx, cy]{GetCenter()};
  SDL_Rect CenterRect{Utilities::Round({
    cx - 3, cy - 3, 6, 6
  })};
  SDL_FillSurfaceRect(Surface, &CenterRect, Color);
}


bool CollisionComponent::GetCollisionRectangle(
  const CollisionComponent& Other,
  SDL_FRect* OutIntersection
) const {
  if (!OutIntersection) {
    std::cerr << "Error: OutIntersection pointer "
      "is null in GetCollisionRectangle.\n";
    return false;
  }

  const SDL_FRect& A{GetBounds()};
  const SDL_FRect& B{Other.GetBounds()};

  return SDL_GetRectIntersectionFloat(
    &A, &B, OutIntersection
  );
}
