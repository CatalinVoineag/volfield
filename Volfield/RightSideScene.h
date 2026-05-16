#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include "../Engine/Scene.h"
#include "MenuButtons.h"

class RightSideScene : public Scene {
public:
  RightSideScene(Window& ParentWindow, int W, int H, int Y)
    : Scene{ParentWindow}, Width{W}, Height{H}, Y{Y} {}

  void Render(SDL_Surface* Surface, float DeltaTime) {
    const auto* Fmt{SDL_GetPixelFormatDetails(Surface->format)};

    SDL_Rect Rect{Surface->w - Width, Y, Width, Height};
    SDL_FillSurfaceRect(
      Surface, &Rect,
      SDL_MapRGB(Fmt, nullptr, 0, 0, 0)
    );

    PlayButton PlayBtn{
      *this,
      Surface->w - Width,
      Height/2 - PlayButton::HEIGHT / 2
    };

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
};
