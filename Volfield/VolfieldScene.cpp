#include <SDL3/SDL_surface.h>
#include <format>
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

  float ShipX = static_cast<int>((ship->GetCenter().x - Info->DestRect.x) * Info->SourceRect.w / Info->DestRect.w);
  int middle = Info->SourceRect.w / 2;
  bool CutLeft{
    ShipX < middle
  };
  std::cout << "MIDDLE " << middle << " ShipX" << ShipX << "\n";
  SDL_Rect cut_rect;
  ship->ClearPath();


//   The scanline approach I described still uses SDL_FillSurfaceRect — just called once per row instead of once for the whole shape:
// for each row Y between path_min_y and path_max_y:
//     compute rightmost_x for that row
//     SDL_FillSurfaceRect(surface, &SDL_Rect{0, Y, rightmost_x, 1}, transparent);
// Each fill is a 1-pixel-tall rect from the left edge to the path boundary. That's all standard SDL — no special method needed.
//
  // Use fill algorithm
  // fill both sides and cut the smallest
  // This only works if the line does devide both fills and it cannot spill
  // quick win is to make the line longer, just to test the algorithm?
  // for (const auto& path : ship->GetPath()) {
  //   // int Y = static_cast<int>(path.y - Info->DestRect.y) * Info->SourceRect.h / Info->DestRect.h;
  //   // int X = static_cast<int>(path.x - Info->DestRect.x) * Info->SourceRect.w / Info->DestRect.w;
  //   // Uint32 transparent = SDL_MapSurfaceRGBA(BackgroundSurface.get(), 0, 0, 0, 0);
  //   // SDL_Rect rect = {0, Y, X, 1};
  //   // SDL_FillSurfaceRect(BackgroundSurface.get(), &rect, transparent);
  //   // SDL_WriteSurfacePixels
  //
  //   SDL_WriteSurfacePixel(BackgroundSurface.get(), path.x, path.y, 255, 0, 0, 1);
  // }


  // This needs more work
  // for (const auto& path : ship->GetPath()) {
  //   if (CutLeft) {
  //     cut_rect = {
  //       0,
  //       static_cast<int>((ship->GetCenter().y - Info->DestRect.y) * Info->SourceRect.h / Info->DestRect.h),
  //
  //       static_cast<int>(path.x - Info->DestRect.x) * Info->SourceRect.w / Info->DestRect.w,
  //       static_cast<int>(path.y - Info->DestRect.y) * Info->SourceRect.h / Info->DestRect.h,
  //     };
  //     std::cout << "CUT Y " << cut_rect.y << " CUT W " << cut_rect.w << " CUT H " << cut_rect.h << "\n";
  //   } else {
  //     cut_rect = {
  //       static_cast<int>(path.x - Info->DestRect.x) * Info->SourceRect.w / Info->DestRect.w,
  //       static_cast<int>((ship->GetCenter().y - Info->DestRect.y) * Info->SourceRect.h / Info->DestRect.h),
  //       static_cast<int>(path.x - Info->DestRect.x) * Info->SourceRect.w / Info->DestRect.w,
  //       static_cast<int>(path.y - Info->DestRect.y) * Info->SourceRect.h / Info->DestRect.h,
  //     };
  //   }
  //
  //   Uint32 transparent = SDL_MapSurfaceRGBA(BackgroundSurface.get(), 0, 0, 0, 0);
  //   SDL_FillSurfaceRect(BackgroundSurface.get(), &cut_rect, transparent);
  // }
}
