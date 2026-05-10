#include "MenuScene.h"

void MenuScene::Load() {
  Entities.clear();

  SetState(GameState::InProgress);
}
