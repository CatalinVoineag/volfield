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

inline std::ostream& operator<<(std::ostream& os, WallPosition position) {
    switch (position) {
        case WallPosition::Top:
            return os << "Top";
        case WallPosition::Bottom:
            return os << "Bottom";
        case WallPosition::Left:
            return os << "Left";
        case WallPosition::Right:
            return os << "Right";
    }

    return os << "Unknown";
}

class VolfieldScene;

class Wall : public Entity {
 public:
  Wall(WallPosition Position, VolfieldScene& Scene)
    : Entity{Scene}, Position{Position} {
      Transform = { AddComponent<TransformComponent>()};
      Collision = {AddComponent<CollisionComponent>()};

      float Width{static_cast<float>(Scene.GetBlitInfo()->DestRect.w)};
      float Height{static_cast<float>(Scene.GetBlitInfo()->DestRect.h)};
      float X = {static_cast<float>(Scene.GetBlitInfo()->DestRect.x)};
      float Y = {static_cast<float>(Scene.GetBlitInfo()->DestRect.y)};
      
      Thickness = {1.0f * Scene.PIXELS_PER_METER};

      using enum WallPosition;
      if (Position == Top) {
        Transform->SetPosition({X, Y - Thickness });
        SetEdge(Transform->GetPosition().y + Thickness);
        Collision->SetSize(Width, Thickness);
      } else if (Position == Bottom) {
        Transform->SetPosition({X, Y + Height});
        SetEdge(Y + Height);
        Collision->SetSize(Width, Thickness);
      } else if (Position == Left) {
        Transform->SetPosition({X - Thickness, Y});
        SetEdge(X - Thickness);
        Collision->SetSize(Thickness, Height);
      } else if (Position == Right) {
        Transform->SetPosition({X + Width, Y});
        SetEdge(X + Width);
        Collision->SetSize(Thickness, Height);
      }
    }

  void SetEdge(int edge) {
    Edge = edge;
  } 

  int GetEdge() {
    return Edge;
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

  TransformComponent* GetTransform() {
    return Transform;
  }

  float GetThickness() {
    return Thickness;
  }

  private:
    WallPosition Position;
    TransformComponent* Transform;
    CollisionComponent* Collision;
    float Thickness;
    int Edge;
};
