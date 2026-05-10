#pragma once
#include <SDL3/SDL.h>
#include <algorithm>
#include <iostream>
#include "Utilities.h"
enum class ScalingMode {
  None, Fill, Contain, Cover
};
struct BlitInfo {
  SDL_Rect SourceRect;
  SDL_Rect DestRect;
};

inline BlitInfo CalculateBlitInfo(
  ScalingMode Mode,
  int SurfaceW, int SurfaceH,
  float TargetX, float TargetY,
  float TargetW, float TargetH
) {
  BlitInfo Info;

  Info.SourceRect = {
    0, 0, SurfaceW, SurfaceH
  };

  if (Mode == ScalingMode::None) {
    Info.DestRect = Utilities::Round({
      TargetX,
      TargetY,
      static_cast<float>(SurfaceW),
      static_cast<float>(SurfaceH)
    });
    return Info;
  }

  if (Mode == ScalingMode::Fill) {
    Info.DestRect = Utilities::Round({
      TargetX, TargetY, TargetW, TargetH
    });
    return Info;
  }

  if (Mode == ScalingMode::Contain) {
    float Scale{
      std::min(TargetW / SurfaceW,
               TargetH / SurfaceH)};
    Info.DestRect =
      Utilities::Round({
        TargetX, TargetY,
        SurfaceW * Scale,
        SurfaceH * Scale
      });

    return Info;
  }

  if (Mode == ScalingMode::Cover) {
    float Scale{
      std::max(TargetW / SurfaceW,
               TargetH / SurfaceH)};
    if (Scale * SurfaceW > TargetW) {
      float ClipW{TargetW / Scale};
      Info.SourceRect =
        Utilities::Round({
          0, 0, ClipW,
          static_cast<float>(SurfaceH)});
    } else if (Scale * SurfaceH > TargetH) {
      float ClipH{TargetH / Scale};
      Info.SourceRect =
        Utilities::Round({
          0, 0, static_cast<float>(SurfaceW),
          ClipH});
    }
    Info.DestRect = Utilities::Round({
      TargetX, TargetY, TargetW, TargetH});
    return Info;
  }

  std::cerr <<
    "Error: Unknown Scaling Mode\n";
  return Info;
}
