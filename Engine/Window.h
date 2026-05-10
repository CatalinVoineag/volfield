#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include "../Config.h"

class Window {
public:
  Window() {
    using namespace Config::Engine;
    SDLWindow = SDL_CreateWindow(
      WINDOW_TITLE.c_str(),
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE
    );
  }

  ~Window() {
    if (SDLWindow) {
      SDL_DestroyWindow(SDLWindow);
    }
  }

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  void Render() {
    auto [r, g, b, a]{
      Config::Engine::WINDOW_BACKGROUND
    };

    const auto* Fmt{SDL_GetPixelFormatDetails(
      GetSurface()->format
    )};

    SDL_FillSurfaceRect(
      GetSurface(), nullptr,
      SDL_MapRGB(Fmt, nullptr, r, g, b)
    );
  }

  void Update() {
    SDL_UpdateWindowSurface(SDLWindow);
  }

  SDL_Surface* GetSurface() {
    return SDL_GetWindowSurface(SDLWindow);
  }

private:
  SDL_Window* SDLWindow;
};

