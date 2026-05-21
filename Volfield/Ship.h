#pragma once
#include <algorithm>
#include <optional>
#include "../Engine/ECS/Entity.h"
#include "../Engine/ECS/ImageComponent.h"
#include "../Engine/ECS/SoundComponent.h"
#include "../Engine/ECS/CollisionComponent.h"
#include "../Engine/ECS/InputComponent.h"
#include "../Engine/ECS/TransformComponent.h"
#include "../Engine/ECS/PhysicsComponent.h"
#include "../Config.h"
#include "../Engine/Vec2.h"

class VolfieldScene;
enum Direction { UP, DOWN, LEFT, RIGHT };

class Ship : public Entity {
  public:
    Ship(
      VolfieldScene& Scene,
      bool Paused
    );

    void HandleEvent(const SDL_Event& E) override;
    void HandleCollision(Entity& Other) override;

    std::vector<int> path; 

    void Tick(float DeltaTime) override {
      Entity::Tick(DeltaTime);
      Physics->SetVelocity({0, 0});
      path.emplace_back(int(Transform->GetPosition().y));
    }

    void Render(SDL_Surface* Surface, float DeltaTime) {
      // if (PreviousXPosition.has_value()) {
      //   int DiffX = *PreviousXPosition - int(Transform->GetPosition().x);
      //   int DiffY = *PreviousYPosition - int(Transform->GetPosition().y);
      //
      //   int lines = std::max(abs(DiffX), abs(DiffY));
      //   int iterations = (lines / 4) + 1;
      //
      //   if (iterations > 0) {
      //     for(int i=0; i < iterations; i++) {
      //       int OffsetX = (direction == RIGHT) ? i * 4 : (direction == LEFT) ? -i * 4 : 0;
      //       int OffsetY = (direction == DOWN) ? i * 4 : (direction == UP) ? -i * 4 : 0;
      //
      //       SDL_Rect PositionIndicator{
      //         int(Transform->GetPosition().x + (Width / 2)) + OffsetX,
      //         int(Transform->GetPosition().y + (Height / 2)) + OffsetY,
      //         4, 4
      //       };
      //
      //       SDL_FillSurfaceRect(
      //         GetScene().Trajectories,
      //         &PositionIndicator,
      //         SDL_MapRGB(
      //           SDL_GetPixelFormatDetails(
      //             GetScene().Trajectories->format),
      //           nullptr, 255, 0, 0
      //         )
      //       );
      //     }
      //   } 
      // } 

      Image->Render(Surface, DeltaTime);

      PreviousXPosition = int(Transform->GetPosition().x);
      PreviousYPosition = int(Transform->GetPosition().y);

      Entity::Render(Surface, DeltaTime);
    }

    Ship& operator=(const Ship& Other) = delete;
    Ship(const Ship& Other) = delete;

    void SetDirection(Direction dir) {
      direction = dir;
    }

  private:
    TransformComponent* Transform;
    ImageComponent* Image;
    CollisionComponent* Collision;
    PhysicsComponent* Physics;
    SoundComponent* Sound;
    InputComponent* Input{nullptr};
    std::optional<int> PreviousXPosition;
    std::optional<int> PreviousYPosition;
    Direction direction = UP;
    int Width;
    int Height;

    void SetIsPaused(bool isPaused) {
      Physics->SetIsEnabled(!isPaused);
      Collision->SetIsEnabled(!isPaused);
    }

    CommandPtr CreateMoveLeftCommand() {
      SetDirection(LEFT);
      using namespace Config::Volfield;
      return std::make_unique<MovementCommand>(
        Vec2{-SHIP_SPEED * Scene::PIXELS_PER_METER, 0.0}
      );
    }
    CommandPtr CreateMoveRightCommand() {
      SetDirection(RIGHT);
      using namespace Config::Volfield;
      return std::make_unique<MovementCommand>(
        Vec2{SHIP_SPEED * Scene::PIXELS_PER_METER, 0.0}
      );
    }
    CommandPtr CreateMoveUpCommand() {
      SetDirection(UP);
      using namespace Config::Volfield;
      return std::make_unique<MovementCommand>(
        Vec2{0.0, -SHIP_SPEED * Scene::PIXELS_PER_METER}
      );
    }
    CommandPtr CreateMoveDownCommand() {
      SetDirection(DOWN);
      using namespace Config::Volfield;
      return std::make_unique<MovementCommand>(
        Vec2{0.0, SHIP_SPEED * Scene::PIXELS_PER_METER}
      );
    }
};

