#include "../Engine/Vec2.h"
#include "VolfieldScene.h"
#include "Ship.h"

Ship::Ship(
    VolfieldScene& Scene,
    bool Paused
    ) : Entity{Scene} {
  Transform = AddComponent<TransformComponent>();
  Transform->SetScale(1.f);

  Image = AddComponent<ImageComponent>("Assets/Ship.png");
  Width = Image->GetWidth();
  Height = Image->GetHeight();

  Transform->SetPosition({
    5.f * Scene::PIXELS_PER_METER,
    5.f * Scene::PIXELS_PER_METER,
    // static_cast<float>(Scene.GetBlitInfo()->DestRect.h + Height / 2)
  });

  Input = AddComponent<InputComponent>();
  Input->UnbindKey(SDLK_SPACE);
  Input->BindKeyHeld(SDLK_A, [this]() { return CreateMoveLeftCommand(); });
  Input->BindKeyHeld(SDLK_D, [this]() { return CreateMoveRightCommand(); });
  Input->BindKeyHeld(SDLK_W, [this]() { return CreateMoveUpCommand(); });
  Input->BindKeyHeld(SDLK_S, [this]() { return CreateMoveDownCommand(); });

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
  float Thickness{.3f * Scene.PIXELS_PER_METER};
  Collision->SetSize(Thickness, Thickness);
  // std::cout << "WIDHT " << Width << " HEIGHT " << Height << "\n";
  Collision->SetOffset(
    // Vec2{static_cast<float>(Width / 2), static_cast<float>(Height / 2)}
    // Vec2{static_cast<float>(Width / 2), 0}
    Vec2{static_cast<float>((Width - Thickness) / 2), (Height - Thickness) / 2 }
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
    SetIsPaused(false);
  } else if (
    E.type == UserEvents::GAME_WON ||
    E.type == UserEvents::GAME_LOST
  ) {
    SetIsPaused(true);
  }
}

void Ship::HandleCollision(Entity& Other) {
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

  TransformComponent* OtherTransform{
    Other.GetComponent<TransformComponent>()
  };

  Vec2 RelativePosition{
    Transform->GetPosition() - OtherTransform->GetPosition()
  };
  
  Vec2 CurrentPos{Transform->GetPosition()};
  if (IsVertical) {
    if (RelativePosition.y > 0) {
      CurrentPos.y += Intersection.h;
    } else {
      CurrentPos.y -= Intersection.h;
    }
  } else {
    if (RelativePosition.x > 0) {
      CurrentPos.x += Intersection.w;
    } else {
      CurrentPos.x -= Intersection.w;
    }
  }
  Transform->SetPosition(CurrentPos);
}
