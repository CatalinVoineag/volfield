#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include "../Engine/Scene.h"
#include "MenuButtons.h"

class HeaderScene : public Scene {
public:
  HeaderScene(Window& ParentWindow, int W, int H)
    : Scene{ParentWindow}, Width{W}, Height{H} {}

  void Render(SDL_Surface* Surface, float DeltaTime) {
    const auto* Fmt{SDL_GetPixelFormatDetails(Surface->format)};

    SDL_Rect Rect{0, 0, Width, Height};
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
    0
  };
};
