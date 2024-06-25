#include <scenes/game.hpp>

// Game scene
GameScene::GameScene(): Scene("game_scene") {}

void GameScene::restart() {
    entities = {};
}