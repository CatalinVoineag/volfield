#pragma once
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
    }

    void Render(SDL_Surface* Surface, float DeltaTime) {

      auto [x, y]{Transform->GetPosition()};
      SDL_Rect PositionIndicator{
        // int(x) - 2, int(y) - 2, 4, 4};
        int(x), int(y), 4, 4};
      SDL_FillSurfaceRect(
        GetScene().Trajectories,
        &PositionIndicator,
        SDL_MapRGB(
          SDL_GetPixelFormatDetails(
            GetScene().Trajectories->format),
          nullptr, 255, 0, 0
        )
      );
      Image->Render(Surface, DeltaTime);
    }

    Ship& operator=(const Ship& Other) = delete;
    Ship(const Ship& Other) = delete;

  private:
    TransformComponent* Transform;
    ImageComponent* Image;
    CollisionComponent* Collision;
    PhysicsComponent* Physics;
    SoundComponent* Sound;
    InputComponent* Input{nullptr};

    void SetIsPaused(bool isPaused) {
      Physics->SetIsEnabled(!isPaused);
      Collision->SetIsEnabled(!isPaused);
    }

    static CommandPtr CreateMoveLeftCommand() {
      using namespace Config::Volfield;
      return std::make_unique<MovementCommand>(
        Vec2{-SHIP_SPEED * Scene::PIXELS_PER_METER, 0.0}
      );
    }
    static CommandPtr CreateMoveRightCommand() {
      using namespace Config::Volfield;
      return std::make_unique<MovementCommand>(
        Vec2{SHIP_SPEED * Scene::PIXELS_PER_METER, 0.0}
      );
    }
    static CommandPtr CreateMoveUpCommand() {
      using namespace Config::Volfield;
      return std::make_unique<MovementCommand>(
        Vec2{0.0, -SHIP_SPEED * Scene::PIXELS_PER_METER}
      );
    }

    static CommandPtr CreateMoveDownCommand() {
      using namespace Config::Volfield;
      return std::make_unique<MovementCommand>(
        Vec2{0.0, SHIP_SPEED * Scene::PIXELS_PER_METER}
      );
    }
};

