#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include "../Engine/Scene.h"
#include "../Engine/ECS/SoundComponent.h"
#include "../Engine/Vec2.h"
#include "../Engine/Blit.h"
#include "HeaderScene.h"
#include "LeftSideScene.h"
#include "RightSideScene.h"
#include "FooterScene.h"

using ScenePtr = std::unique_ptr<Entity>;
using ScenePtrs = std::vector<ScenePtr>;

class VolfieldScene : public Scene {
public:
  VolfieldScene(Window& ParentWindow)
    : Scene{ParentWindow} {
      Load(1, ParentWindow);
    }

  void HandleEvent(const SDL_Event& E) override {
    Scene::HandleEvent(E);
    using namespace UserEvents;
    if (E.type == LAUNCH_LEVEL) {
      Load(E.user.code, GetWindow());
#ifdef ENABLE_CHEATS
    } else if (
      E.type == SDL_EVENT_KEY_DOWN &&
      E.key.key == SDLK_C
    ) {
      // CompleteLevel();
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
      Load(1, GetWindow());
    } else if (E.type == CUT) {
      // Remove tile
      SDL_Rect cut_rect = { 200, 200, 200, 200 };
      Uint32 transparent = SDL_MapSurfaceRGBA(BackgroundSurface.get(), 0, 0, 0, 0);

      SDL_FillSurfaceRect(BackgroundSurface.get(), &cut_rect, transparent);
    }
  }

  void Render(SDL_Surface* Surface, float DeltaTime) {
    Footer->Render(GetWindow().GetSurface(), DeltaTime);
    LeftSide->Render(GetWindow().GetSurface(), DeltaTime);
    RightSide->Render(GetWindow().GetSurface(), DeltaTime);
    Header->Render(GetWindow().GetSurface(), DeltaTime);

    const auto* Fmt{SDL_GetPixelFormatDetails(Surface->format)};

    if (BackgroundSurface) {
      if (!SDL_BlitSurfaceScaled(
        BackgroundSurface.get(), &Info->SourceRect, Surface, &Info->DestRect,
        SDL_SCALEMODE_LINEAR
      )) {
        std::cerr << "Error: Blit failed: "
          << SDL_GetError() << '\n';
      }
    } else {
      SDL_FillSurfaceRect(
        Surface, nullptr,
        SDL_MapRGB(Fmt, nullptr, 20, 50, 20)
      );
    }

    Scene::Render(Surface, DeltaTime);
  }

  void Tick(float DeltaTime) override {
    Scene::Tick(DeltaTime);
  }

  BlitInfo* GetBlitInfo() {
    return Info;
  }

private:
  int LoadedLevel{1};
  void Load(int Level, Window& ParentWindow);
  SoundComponent* WinSound{nullptr};
  std::unique_ptr<Entity> SoundEntity;
  SurfacePtr BackgroundSurface;
  ScenePtrs Scenes;
  HeaderScene* Header{nullptr};
  LeftSideScene* LeftSide{nullptr};
  RightSideScene* RightSide{nullptr};
  FooterScene* Footer{nullptr};
  BlitInfo* Info{nullptr};
};
