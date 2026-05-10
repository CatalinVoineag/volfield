#pragma once
#include <SDL3/SDL.h>
#include <cmath>

namespace Utilities{
  inline SDL_Rect Round(const SDL_FRect& R) {
    return {
      static_cast<int>(std::round(R.x)),
      static_cast<int>(std::round(R.y)),
      static_cast<int>(std::round(R.w)),
      static_cast<int>(std::round(R.h)),
    };
  }

  inline void DrawRectOutline(
    SDL_Surface* Surface,
    const SDL_Rect& Rect,
    Uint32 Color,
    int Thickness = 3
  ) {
    SDL_Rect Top{
      Rect.x, Rect.y, Rect.w, Thickness
    };
    SDL_FillSurfaceRect(Surface, &Top, Color);

    SDL_Rect Bottom{
      Rect.x, Rect.y + Rect.h - Thickness,
      Rect.w, Thickness
    };
    SDL_FillSurfaceRect(Surface, &Bottom, Color);

    int SideHeight{Rect.h - 2 * Thickness};
    SDL_Rect Left{
      Rect.x, Rect.y + Thickness, Thickness,
      SideHeight
    };
    SDL_FillSurfaceRect(Surface, &Left, Color);

    SDL_Rect Right{
      Rect.x + Rect.w - Thickness,
      Rect.y + Thickness, Thickness,
      SideHeight
    };
    SDL_FillSurfaceRect(Surface, &Right, Color);
  }
}
