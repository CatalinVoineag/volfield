#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <SDL3_image/SDL_image.h>
#include <iostream>

struct SDLSurfaceDeleter {
  void operator()(SDL_Surface* Surface) const {
    SDL_DestroySurface(Surface);
  }
};

using SurfacePtr = std::shared_ptr<SDL_Surface>;

using CacheMap =
  std::unordered_map<std::string, SurfacePtr>;

class AssetManager {
 public:
  // Load a surface, reusing if already loaded
  SurfacePtr LoadSurface(const std::string& Path) {
    if (SurfaceCache.contains(Path)) {
      return SurfaceCache[Path];
    }

    SDL_Surface* Surface{IMG_Load(Path.c_str())};
    if (!Surface) {
      std::cerr << "AssetManager failed to load: "
                << Path << " Error: "
                << SDL_GetError() << '\n';
      return nullptr;
    }

    SurfacePtr SharedSurface(
      Surface, SDLSurfaceDeleter{}
    );

    SurfaceCache[Path] = SharedSurface;
    return SharedSurface;
  }

  // Prevent copying/moving
  AssetManager() = default;
  AssetManager(const AssetManager&) = delete;
  AssetManager& operator=(const AssetManager&) = delete;
  AssetManager(AssetManager&&) = delete;
  AssetManager& operator=(AssetManager&&) = delete;

 private:
  CacheMap SurfaceCache;
};
