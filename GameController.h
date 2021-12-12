#ifndef NSU_OOP_2021_LABS_GAMECONTROLLER_H
#define NSU_OOP_2021_LABS_GAMECONTROLLER_H

#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include "GameController.h"
#include "GameModel.h"
#include "ConsoleGameView.h"
#include "GameParticipant.h"
#include "GameParticipantBot.h"
#include "GameParticipantHuman.h"

class GameController {
protected:
    std::shared_ptr<GameModel> model;
    std::shared_ptr<ConsoleGameView> view;

public:
    GameController() {
    }

    void run() {
        do {
            this->model = std::make_shared<GameModel>();
            this->view = std::make_shared<ConsoleGameView>();
            this->view->setGameModel(this->model);

            while (!this->model->isFinished()) {
                std::string line;
                if (this->model->isExpectingInput()) {
                    std::getline(std::cin, line);
                }
                this->model->processInput(line);
            }
        } while (this->model->isContinueWithAnotherParty());
    }
};


#endif //NSU_OOP_2021_LABS_GAMECONTROLLER_H
