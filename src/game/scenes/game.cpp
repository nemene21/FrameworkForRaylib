#include <scenes/game.hpp>

GameScene::GameScene(): Scene("game_scene") {}

void GameScene::restart() {
    entities = {
        new Player()
    };
}