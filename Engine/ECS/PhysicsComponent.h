#pragma once
#include "Component.h"
#include "../Vec2.h"
#include "../Scene.h"

class PhysicsComponent : public Component {
 public:
  using Component::Component;
  void Initialize() override;

  void Tick(float DeltaTime) override;
  void DrawDebugHelpers(SDL_Surface* Surface) override;

  void ApplyForce(const Vec2& Force);
  void ApplyImpulse(const Vec2& Impulse);

  Vec2 GetVelocity() const { return Velocity; }
  void SetVelocity(const Vec2& NewVelocity) {
    Velocity = NewVelocity;
  }

  Vec2 GetGravity() const { return Gravity; }
  void SetGravity(Vec2 NewGravity) {
    Gravity = NewGravity;
  }

  float GetMass() const { return Mass; }
  void SetMass(float NewMass);

  void ConstrainHorizontalMovement(float Left, float Right) {
    ShouldConstrainHorizontalMovement = true;
    ConstrainLeft = Left;
    ConstrainRight = Right;
  }

 private:
  Vec2 Velocity{0.0, 0.0}; // m/s
  Vec2 Acceleration{0.0, 0.0}; // m/s^2
  float Mass{1.0}; // kg
  Vec2 Gravity{Scene::GRAVITY};
  bool ShouldConstrainHorizontalMovement{false};
  float ConstrainLeft{0.0f};
  float ConstrainRight{0.0f};
};
