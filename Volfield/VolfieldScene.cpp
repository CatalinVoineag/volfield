#include <SDL3/SDL_surface.h>
#include <iostream>
#include "VolfieldScene.h"
#include "Ship.h"
#include "Wall.h"

void VolfieldScene::Load(int Level, Window& ParentWindow) {
  Entities.clear();

  BackgroundSurface = GetAssetManager().LoadSurface("Assets/Background1.png");
  Header = new HeaderScene{ ParentWindow, ParentWindow.GetSurface()->w, 100 };
  LeftSide = new LeftSideScene{ ParentWindow, 100, ParentWindow.GetSurface()->h, Header->GetHeight() };
  RightSide = new RightSideScene{ ParentWindow, 100, ParentWindow.GetSurface()->h, Header->GetHeight() };
  Footer = new FooterScene{ ParentWindow, ParentWindow.GetSurface()->w, 100 };

  int SurfaceW = ParentWindow.GetSurface()->w - LeftSide->GetWidth() * 2;
  int SurfaceH = ParentWindow.GetSurface()->h - Header->GetHeight();
  Info = new BlitInfo {
    CalculateBlitInfo(
      ScalingMode::Fill,
      BackgroundSurface->w, BackgroundSurface->h,
      0, 0,
      SurfaceW, SurfaceH
    )};

  Info->DestRect.x = (ParentWindow.GetSurface()->w - Info->DestRect.w) / 2;
  Info->DestRect.y = (ParentWindow.GetSurface()->h - Info->DestRect.h) / 2;

  using enum WallPosition;
  Entities.emplace_back(std::make_unique<Wall>(Top, *this));
  Entities.emplace_back(std::make_unique<Wall>(Left, *this));
  Entities.emplace_back(std::make_unique<Wall>(Bottom, *this));
  Entities.emplace_back(std::make_unique<Wall>(Right, *this));
  Entities.emplace_back(std::make_unique<Ship>(*this, true));

  // using enum WallPosition;
  // Entities.emplace_back(std::make_unique<Wall>(Top, *this, Entities.size()));
  // Entities.emplace_back(std::make_unique<Wall>(Left, *this, Entities.size()));
  // Entities.emplace_back(std::make_unique<Wall>(Bottom, *this, Entities.size()));
  // Entities.emplace_back(std::make_unique<Wall>(Right, *this, Entities.size()));
  // Entities.emplace_back(std::make_unique<Paddle>(*this));

  // std::string FileName{std::format("Assets/Level{}.bin", Level)};
  // SDL_IOStream* Handle{
  //   SDL_IOFromFile(FileName.c_str(), "rb")
  // };
  //
  // if (!Handle) {
  //   CheckSDLError("Loading Level");
  //   return;
  // }
  //
  // Uint8 FileVersion{0};
  // SDL_ReadU8(Handle, &FileVersion);
  //
  // Uint8 GridWidth{0};
  // SDL_ReadU8(Handle, &GridWidth);
  //
  // Uint8 GridHeight{0};
  // SDL_ReadU8(Handle, &GridHeight);
  //
  // Uint32 BlockCount{0};
  // SDL_ReadU32LE(Handle, &BlockCount);
  //
  // std::cout << std::format(
  //   "Loading a version "
  //   "{} level ({}x{}) with {} blocks\n",
  //   FileVersion, GridWidth, GridHeight, BlockCount
  // );
  //
  // for (size_t i{0}; i < BlockCount; i++) {
  //   Entities.emplace_back(
  //     std::make_unique<Block>(Handle, *this)
  //   );
  // }
  // SDL_CloseIO(Handle);
  // BlocksRemaining = BlockCount;
  // LoadedLevel = Level;
  SetState(GameState::InProgress);
}

void VolfieldScene::HandleCutEvent(const SDL_Event& E) {
  Ship* ship = static_cast<Ship*>(E.user.data1);
  std::vector<bool> boolMap(Info->DestRect.w * Info->DestRect.h, false); 

  std::vector<IntVec2> LeftSide;
  DFS(
    Info->DestRect.x, // Start 0
    Info->DestRect.x, // Min
    Info->DestRect.w + Info->DestRect.x, // MaxX
    Info->DestRect.y, // Start 0
    Info->DestRect.y, // Min
    Info->DestRect.h + Info->DestRect.y, // MaxY
    Info->DestRect.w, // Width
    Trajectories, 
    LeftSide,
    boolMap
   );

  std::vector<IntVec2> RightSide;

  DFS(
    (Info->DestRect.w + Info->DestRect.x - 1), // Start right side
    Info->DestRect.x, // Min 
    Info->DestRect.w + Info->DestRect.x, // MaxX
    (Info->DestRect.h + Info->DestRect.y - 1), // Start bottom side
    Info->DestRect.y, // Min
    Info->DestRect.h + Info->DestRect.y, // MaxY
    Info->DestRect.w, // Width
    Trajectories,
    RightSide,
    boolMap
   );

  if (RightSide.size() < LeftSide.size()) {
    for (auto vec2 : RightSide) {
      SDL_WriteSurfacePixel(Trajectories, vec2.x, vec2.y, 255, 0, 0, 255);
    }
  } else {
    for (auto vec2 : LeftSide) {
      SDL_WriteSurfacePixel(Trajectories, vec2.x, vec2.y, 255, 0, 0, 255);
    }
  }

  ship->ClearPath();
}

void VolfieldScene::DFS(
  int X,
  int MinX,
  int MaxX,
  int Y,
  int MinY,
  int MaxY,
  int Width,
  SDL_Surface* surface,
  std::vector<IntVec2> &pixelsToFill,
  std::vector<bool> &boolMap
) {
  if (X < MinX || X >= MaxX || Y < MinY || Y >= MaxY) { 
    return;
  }

  Uint8 r, g, b, a;
  SDL_ReadSurfacePixel(surface, X, Y, &r, &g, &b, &a);
  bool red = r == 255 && g == 0 && b == 0;
  if (red) { 
    return;
  }

  int boolMapIndex = (Y - MinY) * Width + (X - MinX);
  bool visited = boolMap[boolMapIndex];
  if (visited) { 
    return;
  }


  // - Queue (BFS): spreads evenly in all directions like ripples in water
  // This works fine but if the ship is at the edge, you'll get stack overflow.
  // The recursive nature of the function is too deep. It has too many calls to
  // figure out what to cut
  boolMap[boolMapIndex] = true;
  pixelsToFill.emplace_back(IntVec2{X, Y});

  DFS(X+1, MinX, MaxX, Y, MinY, MaxY, Width, surface, pixelsToFill, boolMap);
  DFS(X-1, MinX, MaxX, Y, MinY, MaxY, Width, surface, pixelsToFill, boolMap);
  DFS(X, MinX, MaxX, Y+1, MinY, MaxY, Width, surface, pixelsToFill, boolMap);
  DFS(X, MinX, MaxX, Y-1, MinY, MaxY, Width, surface, pixelsToFill, boolMap);
}
