#include "../Engine/Vec2.h"
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
    static_cast<float>(Scene.GetBlitInfo()->DestRect.h + Height / 2)
  });
  SetCenter();

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

  Collision = AddComponent<CollisionComponent>();
  float Thickness{1};
  Collision->SetSize(Thickness, Thickness);
  Collision->SetOffset(
    Vec2{
      static_cast<float>((Width - Thickness) / 2),
      static_cast<float>((Height - Thickness) / 2)
    }
  );

  // ShipCollision = AddComponent<CollisionComponent>();
  // ShipCollision->SetSize(Width, Height);
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

  Wall* WallPtr = dynamic_cast<Wall*>(&Other);

  if (WallPtr) {
    if (state == CUTTING) {
      using namespace UserEvents;
      SDL_Event E;
      E.type = CUT;
      E.user.data1 = static_cast<void*>(this);
      SDL_PushEvent(&E);
      SetState(SAFE);

      // SDL_FillSurfaceRect(
      //   GetScene().Trajectories,
      //   nullptr,
      //   SDL_MapRGBA(
      //     SDL_GetPixelFormatDetails(
      //       GetScene().Trajectories->format
      //     ),
      //     nullptr, 0, 0, 0, 0
      //   )
      // );
    }
    directions.push_back(WallPtr->GetPosition());
  }
  
  Vec2 CurrentPos{Transform->GetPosition()};
  if (WallPtr->GetPosition() == WallPosition::Top) {
    CurrentPos.y = WallPtr->GetEdge() - (Height / 2.f);
  } else if (WallPtr->GetPosition() == WallPosition::Bottom) {
    CurrentPos.y = WallPtr->GetEdge() - (Height / 2.f);
    // CurrentPos.y = 325.5;
  } else if (WallPtr->GetPosition() == WallPosition::Left ) {
    CurrentPos.x = WallPtr->GetEdge() + (Width / 2.f);
  } else if (WallPtr->GetPosition() == WallPosition::Right) {
    CurrentPos.x = WallPtr->GetEdge() - (Width / 2.f);
  }

  Transform->SetPosition(CurrentPos);
  Collision->RefreshBounds();
}
