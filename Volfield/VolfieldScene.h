#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include "../Engine/Scene.h"
#include "../Engine/ECS/SoundComponent.h"
#include "../Engine/Vec2.h"

class VolfieldScene : public Scene {
public:
  VolfieldScene(Window& ParentWindow)
    : Scene{ParentWindow} {
      Load(1);
    }

  void HandleEvent(const SDL_Event& E) override {
    Scene::HandleEvent(E);
    using namespace UserEvents;
    if (E.type == LAUNCH_LEVEL) {
      Load(E.user.code);
#ifdef ENABLE_CHEATS
    else if (
      E.type == SDL_EVENT_KEY_DOWN &&
      E.key.key == SDLK_C
    ) {
      CompleteLevel();
    }
#endif
    } else if (E.type == GAME_WON) {
      SetState(GameState::Won);
    } else if (E.type == GAME_LOST) {
      SetState(GameState::Lost);
    } else if (
      E.type == SDL_EVENT_KEY_DOWN &&
      E.key.key == SDLK_R
    ) {
      SetState(GameState::InProgress);
      Load(1);
    }
  }

  void Render(SDL_Surface* Surface, float DeltaTime) {
    const auto* Fmt{SDL_GetPixelFormatDetails(Surface->format)};

    SDL_FillSurfaceRect(
      Surface, nullptr,
      SDL_MapRGB(Fmt, nullptr, 20, 50, 20)
    );

    Scene::Render(Surface, DeltaTime);
  }

  void Tick(float DeltaTime) override {
    Scene::Tick(DeltaTime);
  }


private:
  int LoadedLevel{1};
  void Load(int Level);
  SoundComponent* WinSound{nullptr};
  std::unique_ptr<Entity> SoundEntity;
};
