#ifndef NSU_OOP_2021_LABS_GAMEMODELCHANGEHANDLER_H
#define NSU_OOP_2021_LABS_GAMEMODELCHANGEHANDLER_H

#include <string>

class GameModelChangeHandler {
public:
    GameModelChangeHandler() {}

    virtual void onGameModelChanged() = 0;

    virtual void onGameModelError(std::string message) = 0;
};


#endif //NSU_OOP_2021_LABS_GAMEMODELCHANGEHANDLER_H
