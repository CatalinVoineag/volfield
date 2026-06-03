#pragma once
#include <algorithm>
#include <optional>
#include <vector>
#include <cmath>
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
enum State { SAFE, ARMED, CUTTING };

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

      if (directions.size() == 1) {
        std::cout  << "DIRECTIOn " << directions.front() << "\n";
      } else if (directions.size() == 2) {
        std::cout  << "DIRECTIOn break\n\n";
        std::cout  << "DIRECTIOn " << directions.front() << "\n";
        std::cout  << "DIRECTIOn " << directions.back() << "\n";
      }
      directions.clear();

      const bool* CurrentKeyStates{
        SDL_GetKeyboardState(nullptr)};
      SDL_Scancode Scancode{SDL_GetScancodeFromKey(SDLK_C, nullptr)};

      if (CurrentKeyStates[Scancode]) {
        SetState(ARMED);
      } else if (!CurrentKeyStates[Scancode] && state == ARMED) {
        SetState(SAFE);
      }
      SetCenter();
    }

    void Render(SDL_Surface* Surface, float DeltaTime) {
      // This should start drawing from the colision box
      // in the middle of the ship
      if (state != SAFE && directions.size() == 0) {
        SetState(CUTTING);
        int DiffX = PreviousXPosition.value_or(0) - int(Transform->GetPosition().x);
        int DiffY = PreviousYPosition.value_or(0) - int(Transform->GetPosition().y);

        int pixels = std::max(abs(DiffX), abs(DiffY));
        int iterations = pixels + 1;

        for(int i = 0; i <= iterations; i++) {
          float t = float(i) / iterations;
          SDL_Rect PositionIndicator{
            int(*PreviousXPosition - DiffX * t + Width / 2.f),
            int(*PreviousYPosition - DiffY * t + Height / 2.f),
            1, 1
          };

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
      }

      Image->Render(Surface, DeltaTime);

      PreviousXPosition = int(Transform->GetPosition().x);
      PreviousYPosition = int(Transform->GetPosition().y);

      Entity::Render(Surface, DeltaTime);
    }

    Ship& operator=(const Ship& Other) = delete;
    Ship(const Ship& Other) = delete;

    std::vector<Vec2> GetPath() {
      return path;
    }

    void ClearPath() {
      path.clear();
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

    // Probably should remove this
    void SetCenter() {
      Center = {
        GetTransform()->GetPosition().x + (GetWidth() / 2),
        GetTransform()->GetPosition().y + (GetHeight() / 2)
      };
    }
    // Probably should remove this
    Vec2 GetCenter() {
      return Center;
    }

    void SetState(State NewState) {
      if (state == CUTTING && NewState == ARMED) { return; }
      state = NewState;
    }

    State GetState() {
      return state;
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
    State state = SAFE;
    int Width;
    int Height;
    std::vector<WallPosition> directions;
    std::vector<Vec2> path; 
    Vec2 Center;

    bool MoveLeftOrRight() {
      if (state != SAFE) { return true; }

      for (auto w : directions) {
        if (w == WallPosition::Top || w == WallPosition::Bottom)
          return true;
      }
      return false;
    }

    bool MoveUpOrDown() {
      if (state != SAFE) { return true; }

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

