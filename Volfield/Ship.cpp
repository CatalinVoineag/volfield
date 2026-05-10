#include "../Engine/Vec2.h"
#include "VolfieldScene.h"
#include "Ship.h"

Ship::Ship(
    VolfieldScene& Scene,
    bool Paused
    ) : Entity{Scene} {
  Transform = AddComponent<TransformComponent>();
  Transform->SetPosition({
    6.f * Scene::PIXELS_PER_METER,
    6.f * Scene::PIXELS_PER_METER
  });
  Transform->SetScale(1.5f);

  Image = AddComponent<ImageComponent>("Assets/Ship.png");

  Input = AddComponent<InputComponent>();
  Input->UnbindKey(SDLK_SPACE);
  Input->BindKeyHeld(SDLK_A, CreateMoveLeftCommand);
  Input->BindKeyHeld(SDLK_D, CreateMoveRightCommand);
  Input->BindKeyHeld(SDLK_W, CreateMoveUpCommand);
  Input->BindKeyHeld(SDLK_S, CreateMoveDownCommand);

  Physics = AddComponent<PhysicsComponent>();
  Physics->SetGravity({0.f, 0.f});
  Physics->SetVelocity(
    Vec2{1.f, 2.f}.Normalize() 
    * Config::Volfield::SHIP_SPEED
    * Scene::PIXELS_PER_METER
  );
  Physics->ConstrainHorizontalMovement(
    0,
    Scene.GetWidth() - Image->GetWidth()
  );

  Collision = AddComponent<CollisionComponent>();
  Collision->SetSize(
    Image->GetWidth(),
    Image->GetHeight()
  );
  // Sound = AddComponent<SoundComponent>("Assets/ball_collision.wav");

  SetIsPaused(Paused);
}

void Ship::HandleEvent(const SDL_Event& E) {
  if (
      E.type == SDL_EVENT_KEY_DOWN &&
      E.key.key == SDLK_SPACE &&
      GetScene().GetState() == GameState::InProgress
  ) {
    std::cout << "here\n";
    SetIsPaused(false);
  } else if (
    E.type == UserEvents::GAME_WON ||
    E.type == UserEvents::GAME_LOST
  ) {
    SetIsPaused(true);
  }
}

void Ship::HandleCollision(Entity& Other) {
  // if (!dynamic_cast<Block*>(&Other)) {
  //   // Sound->Play();
  // }
  // if (dynamic_cast<Paddle*>(&Other)) {
  //   return;
  // }

  SDL_FRect Intersection;
  Collision->GetCollisionRectangle(
    *Other.GetComponent<CollisionComponent>(),
    &Intersection
  );

  if (!(Physics && Transform)) return;

  // Determine collision axis based on intersection shape
  bool IsVertical{
    Intersection.w > Intersection.h
  };

  // 1. Push ball out of the object to prevent sticking
  Vec2 CurrentPos{Transform->GetPosition()};
  if (IsVertical) {
    if (Physics->GetVelocity().y < 0)
      CurrentPos.y += Intersection.h;
    else
      CurrentPos.y -= Intersection.h;
  } else {
    if (Physics->GetVelocity().x > 0)
      CurrentPos.x -= Intersection.w;
    else
      CurrentPos.x += Intersection.w;
  }
  Transform->SetPosition(CurrentPos);

  TransformComponent* OtherTransform{
    Other.GetComponent<TransformComponent>()
  };

  // Calculate relative position to determine the normal
  Vec2 RelativePosition{
    Transform->GetPosition() - OtherTransform->GetPosition()
  };

  Vec2 Normal {
    IsVertical ? Vec2{0.0f, (RelativePosition.y > 0) ? 1.0f : -1.0f}
    : Vec2{(RelativePosition.x > 0) ? 1.0f : -1.0f, 0.0f}
  };

  Vec2 Velocity{Physics->GetVelocity()};
  // Calculate dot product
  float DotProduct {
    Velocity.x * Normal.x + Velocity.y * Normal.y
  };

  // Only bounce if moving towards the surface
  if (DotProduct < 0) {
    Physics->SetVelocity(
      Velocity - (2 * DotProduct * Normal)
    );
  }
}


