#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "../Config.h"

class Text {
  public:
    Text(
      const std::string& InitialText,
      int FontSize
    ) : Content(InitialText) {
      Font = TTF_OpenFont(
        Config::FONT.c_str(), float(FontSize)
      );
      CheckSDLError("Opening Font");
      SetText(InitialText);
    }

    ~Text() {
      if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        return;
      }
      if (TextSurface) {
        SDL_DestroySurface(TextSurface);
      }
      if (Font) {
        TTF_CloseFont(Font);
      }
    }

    Text(const Text&) = delete;
    Text& operator=(const Text&) = delete;

    void SetText(const std::string& NewText) {
      Content = NewText;

      if (TextSurface) {
        SDL_DestroySurface(TextSurface);
      }
      TextSurface = TTF_RenderText_Blended(
        Font, Content.c_str(), 0, Config::FONT_COLOR
      );
      CheckSDLError("Creating Text Surface");
    }

    void Render(
      SDL_Surface* Surface, SDL_Rect* Rect
    ) {
      if (!TextSurface) return;

      int TextW{TextSurface->w};
      int TextH{TextSurface->h};

      // Center the text
      SDL_Rect Destination {
        Rect->x + (Rect->w - TextW) / 2,
        Rect->y + (Rect->h - TextH) / 2,
        TextW, TextH
      };

      SDL_BlitSurface(
        TextSurface, nullptr,
        Surface, &Destination
      );
    }

  private:
    std::string Content;
    TTF_Font* Font{nullptr};
    SDL_Surface* TextSurface{nullptr};
};
