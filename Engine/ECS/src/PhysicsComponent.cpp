#include <iostream>
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/PhysicsComponent.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/Scene.h"
#include "Engine/Utilities.h"

void PhysicsComponent::Initialize() {
  if (!GetOwner()->GetComponent<TransformComponent>()) {
    std::cerr << "Error: PhysicsComponent "
      "requires TransformComponent on its Owner\n";

    GetOwner()->RemoveComponent(this);
  }
}

void PhysicsComponent::SetMass(float NewMass) {
  if (NewMass <= 0.0) {
    std::cerr << "Error: Mass must be positive. "
                 "Setting to 1.0kg instead\n";
    Mass = 1.0;
  } else {
    Mass = NewMass;
  }
}

void PhysicsComponent::ApplyForce(
  const Vec2& Force
) {
  if (Mass > 0.0f) {
    Acceleration += Force / Mass;
  }
}

void PhysicsComponent::ApplyImpulse(
  const Vec2& Impulse
) {
  if (Mass > 0.0f) {
    Velocity += Impulse / Mass;
  }
}

void PhysicsComponent::Tick(float DeltaTime) {
  if (DeltaTime > 0.1) return;
  if (!GetIsEnabled()) return;
  ApplyForce(Gravity * Mass);
  Velocity += Acceleration * DeltaTime;
  SetOwnerPosition(
    GetOwnerPosition() + Velocity * DeltaTime
  );

  Acceleration = {0.0, 0.0};

  if (ShouldConstrainHorizontalMovement) {
    auto [x, y]{GetOwnerPosition()};
    if (x < ConstrainLeft) {
      SetOwnerPosition({ConstrainLeft, y});
    } else if (x > ConstrainRight) {
      SetOwnerPosition({ConstrainRight, y});
    }
  }
}

void PhysicsComponent::DrawDebugHelpers(
  SDL_Surface* Surface
) {
#ifdef DRAW_DEBUG_HELPERS
  auto [x, y]{GetOwnerPosition()};
  SDL_Rect PositionIndicator{
    int(x) - 2, int(y) - 2, 4, 4};
  SDL_FillSurfaceRect(
    GetScene().Trajectories,
    &PositionIndicator,
    SDL_MapRGB(
      SDL_GetPixelFormatDetails(
        GetScene().Trajectories->format),
      nullptr, 0, 0, 255
    )
  );
#endif
}
