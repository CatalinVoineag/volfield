#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <algorithm>
#include "Engine/ECS/ImageComponent.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/Utilities.h"

namespace{
  struct BlitInfo {
    SDL_Rect SourceRect;
    SDL_Rect DestRect;
  };

  BlitInfo CalculateBlitInfo(
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
}

ImageComponent::ImageComponent(
  Entity* Owner,
  const std::string& FilePath
) : Component(Owner),
    ImageFilePath(FilePath
    ) {
  ImageSurface = GetAssetManager()
    .LoadSurface(ImageFilePath);
}

void ImageComponent::Render(SDL_Surface* Surface, float DeltaTime) {
  if (!GetIsEnabled()) return;
  if (!ImageSurface) return;

  auto [TargetX, TargetY]{GetOwnerPosition() + Offset};
  float TargetW{GetWidth() * GetOwnerScale()};
  float TargetH{GetHeight() * GetOwnerScale()};

  int SurfaceW{GetSurfaceWidth()};
  int SurfaceH{GetSurfaceHeight()};

  BlitInfo Info{
    CalculateBlitInfo(
      ScaleMode,
      SurfaceW, SurfaceH,
      TargetX, TargetY,
      TargetW, TargetH
    )};

  if (!SDL_BlitSurfaceScaled(
    ImageSurface.get(),
    &Info.SourceRect,
    Surface,
    &Info.DestRect,
    SDL_SCALEMODE_LINEAR
  )) {
    std::cerr << "Error: Blit failed: "
      << SDL_GetError() << '\n';
  }
}

void ImageComponent::DrawDebugHelpers(
  SDL_Surface* Surface
) {
  using Utilities::DrawRectOutline;
  if (!GetIsEnabled()) return;
  if (!ImageSurface) return;

  // Calculate position and dimensions, similar to Render()
  auto [TargetX, TargetY]{GetOwnerPosition() + Offset};
  float OwnerScale{GetOwnerScale()};
  float TargetW{GetWidth() * OwnerScale};
  float TargetH{GetHeight() * OwnerScale};
  int SurfaceW{GetSurfaceWidth()};
  int SurfaceH{GetSurfaceHeight()};

  // Natural Bounds (Green Outline)
  SDL_Rect NaturalBounds{
    Utilities::Round({
      TargetX, TargetY,
      SurfaceW * OwnerScale,
      SurfaceH * OwnerScale
    })};

  DrawRectOutline(
    Surface, NaturalBounds,
    SDL_MapRGB(
      SDL_GetPixelFormatDetails(Surface->format),
      nullptr, 0, 255, 0
    )
  );

  // Rendered Bounds (Red Outline)
  BlitInfo Info{CalculateBlitInfo(
    ScaleMode,
    SurfaceW, SurfaceH,
    TargetX, TargetY,
    TargetW, TargetH
  )};

  DrawRectOutline(
    Surface, Info.DestRect,
    SDL_MapRGB(
      SDL_GetPixelFormatDetails(Surface->format),
      nullptr, 255, 0, 0
    )
  );

  // Position marker (Blue Square)
  SDL_Rect DebugRect{
    Utilities::Round({
      TargetX - 5, TargetY - 5, 10, 10})
  };
  SDL_FillSurfaceRect(
    Surface, &DebugRect,
    SDL_MapRGB(
      SDL_GetPixelFormatDetails(Surface->format),
      nullptr, 0, 0, 255
    )
  );
}

bool ImageComponent::LoadNewImage(
  const std::string& NewPath
) {
  ImageFilePath = NewPath;
  ImageSurface = GetAssetManager().LoadSurface(NewPath);
  return ImageSurface != nullptr;
}

int ImageComponent::GetSurfaceWidth() const {
  if (!ImageSurface) {
    std::cerr << "Warning: Attempted to get "
      "width from null ImageSurface.\n";
    return 0;
  }
  return ImageSurface->w;
}

int ImageComponent::GetSurfaceHeight() const {
  if (!ImageSurface) {
    std::cerr << "Warning: Attempted to get "
      "height from null ImageSurface.\n";
    return 0;
  }
  return ImageSurface->h;
}

void ImageComponent::Initialize() {
  Entity* Owner{GetOwner()};
  if (!Owner->GetComponent<TransformComponent>()) {
    std::cout <<
      "Error: ImageComponent requires"
      " TransformComponent on its Owner\n";

    Owner->RemoveComponent(this);
  }
}

void ImageComponent::SetWidth(float NewWidth) {
  Width = NewWidth;
}

void ImageComponent::SetHeight(
  float NewHeight) { Height = NewHeight; }

void ImageComponent::ResetWidth() {
  Width = std::nullopt;
}

void ImageComponent::ResetHeight() {
  Height = std::nullopt;
}

float ImageComponent::GetWidth() const {
  // If Width has a value, return it.
  // Otherwise, return surface width.
  return Width.value_or(GetSurfaceWidth());
}

float ImageComponent::GetHeight() const {
  // If Height has a value, return it.
  // Otherwise, return surface height.
  return Height.value_or(GetSurfaceHeight());
}

void ImageComponent::SetScalingMode(ScalingMode Mode) {
  ScaleMode = Mode;
}
