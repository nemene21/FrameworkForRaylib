#include <framework.hpp>
#include <scenes/game.hpp>

int main() {
    Framework::init("ne_mene's framework", {320, 180}, 2, true);

    new GameScene();
    SceneManager::set_scene("game_scene");

    Framework::run();
    return 0;
}

