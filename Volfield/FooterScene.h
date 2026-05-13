#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include "../Engine/Scene.h"
#include "MenuButtons.h"

class FooterScene : public Scene {
public:
  FooterScene(Window& ParentWindow, int W, int H)
    : Scene{ParentWindow}, Width{W}, Height{H} {}

  void Render(SDL_Surface* Surface, float DeltaTime) {
    const auto* Fmt{SDL_GetPixelFormatDetails(Surface->format)};

    SDL_Rect Rect{0, Config::Engine::WINDOW_HEIGHT - Height, Width, Height};
    SDL_FillSurfaceRect(
      Surface, &Rect,
      SDL_MapRGB(Fmt, nullptr, 0, 0, 0)
    );

    PlayBtn.Render(Surface);
  }

  void Tick(float DeltaTime) override {
    Scene::Tick(DeltaTime);
  }

  int GetWidth() { return Width; }
  int GetHeight() { return Height; }

private:
  int Width;
  int Height;
  PlayButton PlayBtn{
    *this,
    Width/2 - PlayButton::WIDTH / 2,
    Config::Engine::WINDOW_HEIGHT - Height / 2
  };
};
