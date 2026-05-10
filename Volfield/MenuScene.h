#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include "../Engine/Scene.h"
#include "../Engine/ECS/SoundComponent.h"
#include "../Engine/Vec2.h"
#include "MenuButtons.h"

class MenuScene : public Scene {
public:
  MenuScene(Window& ParentWindow)
    : Scene{ParentWindow} {
      Load();
    }

  void HandleEvent(const SDL_Event& E) override {
    PlayBtn.HandleEvent(E);
    SettingsBtn.HandleEvent(E);
  }

  void Render(SDL_Surface* Surface, float DeltaTime) {
    const auto* Fmt{SDL_GetPixelFormatDetails(Surface->format)};

    SDL_FillSurfaceRect(
      Surface, nullptr,
      SDL_MapRGB(Fmt, nullptr, 0, 0, 0)
    );

    PlayBtn.Render(Surface);
    SettingsBtn.Render(Surface);
    Scene::Render(Surface, DeltaTime);
  }

  void Tick(float DeltaTime) override {
    PlayBtn.Tick(DeltaTime);
    SettingsBtn.Tick(DeltaTime);
    Scene::Tick(DeltaTime);
  }

private:
  void Load();
  PlayButton PlayBtn{*this};
  SettingsButton SettingsBtn{*this};
};
