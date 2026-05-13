#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <iostream>
#include "../Engine/Scene.h"
#include "MenuButtons.h"

class LeftSideScene : public Scene {
public:
  LeftSideScene(Window& ParentWindow, int W, int H, int Y)
    : Scene{ParentWindow}, Width{W}, Height{H}, Y{Y} {}

  void Render(SDL_Surface* Surface, float DeltaTime) {
    const auto* Fmt{SDL_GetPixelFormatDetails(Surface->format)};

    SDL_Rect Rect{0, Y, Width, Height};
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
  int Y;
  PlayButton PlayBtn{
    *this,
    0,
    Height/2 - PlayButton::HEIGHT / 2
  };
};
