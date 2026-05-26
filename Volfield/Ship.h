#pragma once
#include <algorithm>
#include <optional>
#include <vector>
#include "../Engine/ECS/Entity.h"
#include "../Engine/ECS/ImageComponent.h"
#include "../Engine/ECS/SoundComponent.h"
#include "../Engine/ECS/CollisionComponent.h"
#include "../Engine/ECS/InputComponent.h"
#include "../Engine/ECS/TransformComponent.h"
#include "../Engine/ECS/PhysicsComponent.h"
#include "../Config.h"
#include "../Engine/Vec2.h"
#include "Wall.h"

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

    void Tick(float DeltaTime) override {
      Entity::Tick(DeltaTime);
      Physics->SetVelocity({0, 0});
      directions.clear();

      const bool* CurrentKeyStates{
        SDL_GetKeyboardState(nullptr)};
      SDL_Scancode Scancode{SDL_GetScancodeFromKey(SDLK_C, nullptr)};

      if (CurrentKeyStates[Scancode]) {
        SetCut(true);
      }
      SetCenter();
    }

    void Render(SDL_Surface* Surface, float DeltaTime) {
      // This should start drawing from the colision box
      // in the middle of the ship
      if (PreviousXPosition.has_value() && Cut && directions.size() == 0) {
        int DiffX = *PreviousXPosition - int(Transform->GetPosition().x);
        int DiffY = *PreviousYPosition - int(Transform->GetPosition().y);

        int lines = std::max(abs(DiffX), abs(DiffY));
        int iterations = (lines / 4) + 1;

        for(int i = 0; i <= iterations; i++) {
          float t = float(i) / iterations;
          SDL_Rect PositionIndicator{
            int(*PreviousXPosition - DiffX * t + Width / 2),
              int(*PreviousYPosition - DiffY * t + Height / 2),
              4, 4
          };

          path.emplace_back(int(PositionIndicator.y)+4);
          // std::cout << "PATH " << path.back() << "\n";
          path.emplace_back(int(PositionIndicator.y)+3);
          // std::cout << "PATH " << path.back() << "\n";
          path.emplace_back(int(PositionIndicator.y)+2);
          // std::cout << "PATH " << path.back() << "\n";
          path.emplace_back(int(PositionIndicator.y)+1);
          // std::cout << "PATH " << path.back() << "\n";

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
      }

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

    void SetCut(bool cut) {
      Cut = cut;
    }

    std::vector<int> GetPath() {
      return path;
    }

    TransformComponent* GetTransform() {
      return Transform;
    }

    int GetWidth() {
      return Width;
    }

    int GetHeight() {
      return Height;
    }

    void SetCenter() {
      Center = {
        GetTransform()->GetPosition().x + (GetWidth() / 2),
        GetTransform()->GetPosition().y + (GetHeight() / 2)
      };
    }

    Vec2 GetCenter() {
      return Center;
    }

  private:
    TransformComponent* Transform;
    ImageComponent* Image;
    CollisionComponent* Collision;
    CollisionComponent* ShipCollision;
    PhysicsComponent* Physics;
    SoundComponent* Sound;
    InputComponent* Input{nullptr};
    std::optional<int> PreviousXPosition;
    std::optional<int> PreviousYPosition;
    Direction direction = UP;
    int Width;
    int Height;
    std::vector<WallPosition> directions;
    bool Cut = false;
    std::vector<int> path; 
    Vec2 Center;

    bool MoveLeftOrRight() {
      if (Cut) { return true; }

      for (auto w : directions) {
        if (w == WallPosition::Top || w == WallPosition::Bottom)
          return true;
      }
      return false;
    }

    bool MoveUpOrDown() {
      if (Cut) { return true; }

      for (auto w : directions) {
        if (w == WallPosition::Left || w == WallPosition::Right)
          return true;
      }
      return false;
    }

    void SetIsPaused(bool isPaused) {
      Physics->SetIsEnabled(!isPaused);
      Collision->SetIsEnabled(!isPaused);
    }

    CommandPtr CreateMoveLeftCommand() {
      SetDirection(LEFT);
      using namespace Config::Volfield;

      Vec2 moveVector;
      if (MoveLeftOrRight()) {
        moveVector = {-SHIP_SPEED * Scene::PIXELS_PER_METER, 0.0};
      } else {
        moveVector = {0.0, 0.0};
      }

      return std::make_unique<MovementCommand>(moveVector);
    }
    CommandPtr CreateMoveRightCommand() {
      SetDirection(RIGHT);
      using namespace Config::Volfield;


      Vec2 moveVector;
      if (MoveLeftOrRight()) {
        moveVector = {SHIP_SPEED * Scene::PIXELS_PER_METER, 0.0};
      } else {
        moveVector = {0.0, 0.0};
      }
      return std::make_unique<MovementCommand>(moveVector);
    }
    CommandPtr CreateMoveUpCommand() {
      SetDirection(UP);
      using namespace Config::Volfield;

      Vec2 moveVector;
      if (MoveUpOrDown()) {
        moveVector = {0.0, -SHIP_SPEED * Scene::PIXELS_PER_METER};
      } else {
        moveVector = {0.0, 0.0};
      }

      return std::make_unique<MovementCommand>(moveVector);
    }
    CommandPtr CreateMoveDownCommand() {
      SetDirection(DOWN);
      using namespace Config::Volfield;

      Vec2 moveVector;
      if (MoveUpOrDown()) {
        moveVector = {0.0, SHIP_SPEED * Scene::PIXELS_PER_METER};
      } else {
        moveVector = {0.0, 0.0};
      }

      return std::make_unique<MovementCommand>(moveVector);
    }
};

