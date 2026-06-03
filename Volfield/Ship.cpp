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
    // The edge of the Bottom wall
    Scene.GetBlitInfo()->DestRect.y + Scene.GetBlitInfo()->DestRect.h - (Height / 2.f)
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

  Vec2 CurrentPos{Transform->GetPosition()};
  if (WallPtr->GetPosition() == WallPosition::Top) {
    CurrentPos.y = WallPtr->GetEdge() - (Height / 2.f);
  } else if (WallPtr->GetPosition() == WallPosition::Bottom) {
    CurrentPos.y = WallPtr->GetEdge() - (Height / 2.f);
  } else if (WallPtr->GetPosition() == WallPosition::Left ) {
    CurrentPos.x = WallPtr->GetEdge() - (Width / 2.f);
  } else if (WallPtr->GetPosition() == WallPosition::Right) {
    CurrentPos.x = WallPtr->GetEdge() - (Width / 2.f);
  }

  Transform->SetPosition(CurrentPos);
  Collision->RefreshBounds();

  if (WallPtr) {
    if (state == CUTTING) {
      using namespace UserEvents;
      SDL_Event E;
      E.type = CUT;
      E.user.data1 = static_cast<void*>(this);
      SDL_PushEvent(&E);
      SetState(SAFE);

      int diff;
      if (WallPtr->GetPosition() == WallPosition::Top || WallPtr->GetPosition() == WallPosition::Bottom) {
        diff = abs(path.back().y - WallPtr->GetEdge());
      } else {
        diff = abs(path.back().x - WallPtr->GetEdge());
      }

      for (int i=0; i < diff; i++) {
        int x;
        int y;

        if (WallPtr->GetPosition() == WallPosition::Top) {
          y = path.back().y - 1;
          x = path.back().x;
        } else if (WallPtr->GetPosition() == WallPosition::Bottom) {
          y = path.back().y + 1;
          x = path.back().x;
        } else if (WallPtr->GetPosition() == WallPosition::Left) {
          y = path.back().y;
          x = path.back().x - 1;
        } else if (WallPtr->GetPosition() == WallPosition::Right) {
          y = path.back().y;
          x = path.back().x + 1;
        }

        SDL_Rect PositionIndicator{x, y, 1, 1};

        path.emplace_back(Vec2{float(PositionIndicator.x), float(PositionIndicator.y) });

        SDL_FillSurfaceRect(
          GetScene().Trajectories,
          &PositionIndicator,
          SDL_MapRGB(
            SDL_GetPixelFormatDetails(
              GetScene().Trajectories->format),
            nullptr, 255, 0, 0
          )
        );
      }

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
}
