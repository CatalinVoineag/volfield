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
  // if (ship->GetPath().size() > 0) {
  //   std::cout << ship->GetPath().back() << " this \n";
  // }
  //

  SDL_Rect cut_rect = {
    static_cast<int>((ship->GetCenter().x - Info->DestRect.x) * Info->SourceRect.w / Info->DestRect.w),
    static_cast<int>((ship->GetCenter().y - Info->DestRect.y) * Info->SourceRect.h / Info->DestRect.h),
    // Info->DestRect.w - ship->GetTransform()->GetPosition().x,
    Info->SourceRect.w,
    Info->SourceRect.h
  };

  Uint32 transparent = SDL_MapSurfaceRGBA(BackgroundSurface.get(), 0, 0, 0, 0);
  SDL_FillSurfaceRect(BackgroundSurface.get(), &cut_rect, transparent);
}
