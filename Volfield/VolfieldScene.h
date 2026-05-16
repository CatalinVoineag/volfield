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
      Load(1);
    }

  void HandleEvent(const SDL_Event& E) override {
    Scene::HandleEvent(E);
    using namespace UserEvents;
    if (E.type == LAUNCH_LEVEL) {
      Load(E.user.code);
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
      Load(1);
    } else if (E.type == SDL_EVENT_KEY_DOWN && E.key.key == SDLK_F) {
      // Remove tile
      SDL_Rect cut_rect = { 200, 200, 200, 200 };
      Uint32 transparent = SDL_MapSurfaceRGBA(BackgroundSurface.get(), 0, 0, 0, 0);

      SDL_FillSurfaceRect(BackgroundSurface.get(), &cut_rect, transparent);
    }
  }

  void Render(SDL_Surface* Surface, float DeltaTime) {
    HeaderScene Header{
      GetWindow(),
      Surface->w,
      100
    };

    LeftSideScene LeftSide{
      GetWindow(),
      100,
      Surface->h,
      Header.GetHeight()
    };

    RightSideScene RightSide{
      GetWindow(),
      100,
      Surface->h,
      Header.GetHeight()
    };
    FooterScene Footer{
      GetWindow(),
      Surface->w,
      100
    };

    Footer.Render(GetWindow().GetSurface(), DeltaTime);
    LeftSide.Render(GetWindow().GetSurface(), DeltaTime);
    RightSide.Render(GetWindow().GetSurface(), DeltaTime);
    Header.Render(GetWindow().GetSurface(), DeltaTime);

    const auto* Fmt{SDL_GetPixelFormatDetails(Surface->format)};

    int SurfaceW = Surface->w - LeftSide.GetWidth() * 2;
    int SurfaceH = Surface->h - Header.GetHeight();

    if (BackgroundSurface) {
      BlitInfo Info{
        CalculateBlitInfo(
          ScalingMode::Fill,
          BackgroundSurface->w, BackgroundSurface->h,
          0, 0,
          SurfaceW, SurfaceH
        )};

      Info.DestRect.x = (Surface->w - Info.DestRect.w) / 2;
      Info.DestRect.y = (Surface->h - Info.DestRect.h) / 2;

      if (!SDL_BlitSurfaceScaled(
        BackgroundSurface.get(), &Info.SourceRect, Surface, &Info.DestRect,
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

private:
  int LoadedLevel{1};
  void Load(int Level);
  SoundComponent* WinSound{nullptr};
  std::unique_ptr<Entity> SoundEntity;
  SurfacePtr BackgroundSurface;
  ScenePtrs Scenes;
};
