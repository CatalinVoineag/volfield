#pragma once
#include "../Engine/ECS/Entity.h"
#include "../Engine/Scene.h"
#include "../Engine/ECS/TransformComponent.h"
#include "../Engine/ECS/CollisionComponent.h"
#include "Ship.h"
#include "VolfieldScene.h"

enum class WallPosition {
  Top, Bottom, Left, Right
};

class VolfieldScene;

class Wall : public Entity {
 public:
  Wall(WallPosition Position, VolfieldScene& Scene)
    : Entity{Scene}, Position{Position} {
      TransformComponent* Transform{
        AddComponent<TransformComponent>()
      };

      CollisionComponent* Collision{
        AddComponent<CollisionComponent>()
      };

      float Width{static_cast<float>(Scene.GetBlitInfo()->DestRect.w)};
      float Height{static_cast<float>(Scene.GetBlitInfo()->DestRect.h)};
      float X = {static_cast<float>(Scene.GetBlitInfo()->DestRect.x)};
      float Y = {static_cast<float>(Scene.GetBlitInfo()->DestRect.y)};
      
      // float Thickness{2.f};
      float Thickness{1.0f * Scene.PIXELS_PER_METER};


      using enum WallPosition;
      if (Position == Top) {
        Transform->SetPosition({X, Y - Thickness });
        Collision->SetSize(Width, Thickness);
      } else if (Position == Bottom) {
        Transform->SetPosition({X, Y + Height});
        Collision->SetSize(Width, Thickness);
      } else if (Position == Left) {
        Transform->SetPosition({X - Thickness, Y});
        Collision->SetSize(Thickness, Height);
      } else if (Position == Right) {
        Transform->SetPosition({X + Width, Y});
        Collision->SetSize(Thickness, Height);
      }
    }

  // void HandleCollision(Entity& Other) override {
  //   auto& GameScene = static_cast<VolfieldScene&>(GetScene());
  //   Ship* ShipPtr = dynamic_cast<Ship*>(&Other);
  //
  //   if (ShipPtr && Position == WallPosition::Top) {
  //     // for (int i=0; i < ShipPtr->path.size(); i++) {
  //     //   std::cout << "BACK " << i << " " << ShipPtr->path[i] << "\n";
  //     // }
  //
  //     std::vector<int> path = ShipPtr->path; 
  //     auto it = unique(path.begin(), path.end());
  //
  //     path.erase(it, path.end());
  //
  //     SDL_Event E{};
  //     E.type = UserEvents::CUT;
  //     SDL_PushEvent(&E);
  //   }
  // }
  //
  WallPosition GetPosition() {
    return Position;
  }

  private:
    WallPosition Position;
};
