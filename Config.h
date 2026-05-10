#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <string>
#include <vector>

namespace UserEvents{
#ifdef WITH_EDITOR
inline Uint32 ACTOR_DRAG{SDL_RegisterEvents(1)};
inline Uint32 LOAD_LEVEL{SDL_RegisterEvents(1)};
inline Uint32 SAVE_LEVEL{SDL_RegisterEvents(1)};
inline Uint32 SAVE_AND_PLAY_LEVEL{SDL_RegisterEvents(1)};
inline Uint32 LEVEL_EDIT{SDL_RegisterEvents(1)};
#endif
inline Uint32 GAME_WON{SDL_RegisterEvents(1)};
inline Uint32 GAME_LOST{SDL_RegisterEvents(1)};
inline Uint32 COMPLETE_LEVEL{SDL_RegisterEvents(1)};
inline Uint32 LAUNCH_LEVEL{SDL_RegisterEvents(1)};
}

namespace Config {
enum class ActorType : Uint8 {
  Actor = 0,
  BlueBlock = 1,
  GreenBlock = 2,
  CyanBlock = 3,
  OrangeBlock = 4,
  RedBlock = 5,
  YellowBlock = 6,
  StrongYellowBlock = 7,
  StrongRedBlock = 8,
  CrackedYellowBlock = 9,
  CrackedRedBlock = 10,
  TNTRedBlock = 11,
  TNTBlueBlock = 12,
  BallGreenBlock = 13,
};

inline const std::vector BUTTON_COLORS{
  SDL_Color{15, 15, 15, 255},  // Normal
  SDL_Color{15, 155, 15, 255}, // Hover
  SDL_Color{225, 15, 15, 255}, // Active
  SDL_Color{60, 60, 60, 255}   // Disabled
};

inline constexpr SDL_Color FONT_COLOR{
  255, 255, 255, 255
};

inline const std::string BASE_PATH{
  SDL_GetBasePath()
};

inline const std::string FONT{
  "Assets/Rubik-SemiBold.ttf"
};
}

#ifdef WITH_EDITOR
namespace Config::Editor {
inline const Uint8 VERSION{1};
// Level
inline const int HORIZONTAL_GRID_SNAP{50};
inline const int VERTICAL_GRID_SNAP{25};
inline const Uint8 GRID_WIDTH{13};
inline const Uint8 GRID_HEIGHT{6};
inline const int LEVEL_WIDTH{
  HORIZONTAL_GRID_SNAP * GRID_WIDTH
};
inline const int LEVEL_HEIGHT{
  VERTICAL_GRID_SNAP * GRID_HEIGHT
};
inline constexpr SDL_Color LEVEL_BACKGROUND{
  50, 50, 50, 255
};

// ActorMenu
inline const int ACTOR_MENU_WIDTH{70};
inline const int ACTOR_MENU_POSITION_X{LEVEL_WIDTH};
inline const SDL_Color ACTOR_MENU_BACKGROUND{
  15, 15, 15, 255
};
inline const int PADDING{10};

// Window
inline const std::string WINDOW_TITLE{"Editor"};
inline const int WINDOW_WIDTH{
  LEVEL_WIDTH + ACTOR_MENU_WIDTH
};
inline const int WINDOW_HEIGHT{LEVEL_HEIGHT + 250};
inline const SDL_Color WINDOW_BACKGROUND{
  35, 35, 35, 255
};
}
#endif

namespace Config::Volfield {
  // Meters per second
  inline const float SHIP_SPEED{10.f};
  inline const float PADDLE_SPEED{5};
}

namespace Config::Engine {
inline const std::string WINDOW_TITLE{"Volfield"};
inline const int WINDOW_WIDTH{720};
inline const int WINDOW_HEIGHT{400};
inline const SDL_Color WINDOW_BACKGROUND{
  35, 35, 35, 255
};
}

inline void CheckSDLError(const std::string& Msg) {
#ifdef CHECK_ERRORS
  const char* error = SDL_GetError();
  if (*error != '\0') {
    std::cerr << Msg << " Error: "
      << error << '\n';
    SDL_ClearError();
  }
#endif
}

