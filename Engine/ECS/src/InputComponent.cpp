#include <SDL3/SDL.h>
#include "Engine/ECS/InputComponent.h"
#include "Engine/ECS/Commands.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Vec2.h"
#include "Engine/Scene.h"

namespace{
// Speed relative to world meters
const float SPEED{5.f * Scene::PIXELS_PER_METER};

CommandPtr CreateMoveLeftCommand() {
  return std::make_unique<MovementCommand>(
      Vec2{-SPEED, 0.0});
}

CommandPtr CreateMoveRightCommand() {
  return std::make_unique<MovementCommand>(
      Vec2{SPEED, 0.0});
}

CommandPtr CreateJumpCommand() {
    const float JUMP_IMPULSE_MAGNITUDE{
      -500.f * Scene::PIXELS_PER_METER};
    return std::make_unique<JumpCommand>(
      Vec2{0.0, JUMP_IMPULSE_MAGNITUDE});
  }
}

void InputComponent::Initialize() {
  BindKeyHeld(SDLK_LEFT, CreateMoveLeftCommand);
  BindKeyHeld(SDLK_RIGHT,
              CreateMoveRightCommand);
  BindKeyDown(SDLK_SPACE, CreateJumpCommand);
}

void InputComponent::Tick(float DeltaTime) {
  if (!GetIsEnabled()) return;
  Entity* Owner{GetOwner()};
  if (!Owner) return;

  // SDL3 returns const bool*
  const bool* CurrentKeyStates{
    SDL_GetKeyboardState(nullptr)};

  for (const auto& [Key, Factory] :
       KeyHeldBindings) {
    SDL_Scancode Scancode{
      SDL_GetScancodeFromKey(Key, nullptr)};
    if (CurrentKeyStates[Scancode]) {
      Owner->HandleCommand(Factory());
    }
  }
}

void InputComponent::HandleEvent(
  const SDL_Event& E) {
  if (!GetIsEnabled()) return;

  if (E.type == SDL_EVENT_KEY_DOWN) {
    Entity* Owner{GetOwner()};
    if (!Owner) return;
    SDL_Keycode Key{E.key.key};
    if (KeyDownBindings.contains(Key)) {
      Owner->HandleCommand(
        KeyDownBindings[Key]());
    }
  }
}
