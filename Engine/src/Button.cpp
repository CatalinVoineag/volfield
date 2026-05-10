#include "../Button.h"
#include "../Scene.h"
#include "../../Config.h"

void Button::HandleEvent(const SDL_Event& E) {
  using enum ButtonState;
  if (E.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
      E.button.button == SDL_BUTTON_LEFT &&
      State == Hover
  ) {
    HandleLeftClick();
  } else if (
    E.type == SDL_EVENT_MOUSE_MOTION &&
    ParentScene.HasMouseFocus()
  ) {
    SDL_Point Pos{(int)E.motion.x, (int)E.motion.y};
    bool Hovering(SDL_PointInRect(&Pos, &Rect));
    if (State == Normal && Hovering) {
      State = Hover;
    } else if (State == Hover && !Hovering) {
      State = Normal;
    }
  }
}

void Button::Render(SDL_Surface* Surface) {
  using namespace Config;
  auto [r, g, b, a]{
    BUTTON_COLORS[static_cast<int>(State)]
  };

  const auto* Fmt{SDL_GetPixelFormatDetails(
    Surface->format
  )};

  SDL_FillSurfaceRect(
    Surface, &Rect, SDL_MapRGB(
      Fmt, nullptr, r, g, b
    )
  );

  ButtonText.Render(Surface, &Rect);
}

