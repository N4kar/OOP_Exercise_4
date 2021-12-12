#include <memory>
#include "GameController.h"

int main(const int argc, const char* argv[]) {
    auto gameController = std::make_shared<GameController>();
    gameController->run();
    return 0;
}