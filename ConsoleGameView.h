#ifndef NSU_OOP_2021_LABS_CONSOLEGAMEVIEW_H
#define NSU_OOP_2021_LABS_CONSOLEGAMEVIEW_H

#include "GameModel.h"
#include "GameModelChangeHandler.h"
#include <memory>
#include <iostream>

class ConsoleGameView :
        public GameModelChangeHandler,
        public std::enable_shared_from_this<ConsoleGameView> {
protected:
    std::shared_ptr<GameModel> gameModel;

    char convertCellValueToSymbol(int value) {
        switch (value) {
            case GameModel::CELL_VALUE_EMPTY_CELL:
                return EMPTY_CELL_SYMBOL;
            case GameModel::CELL_VALUE_HUMAN_CELL:
                return PLAYER_SYMBOL_HUMAN;
            case GameModel::CELL_VALUE_BOT_CELL:
                return PLAYER_SYMBOL_BOT;
            default:
                throw GameBaseException("Unreachable section");
        }
    }

public:
    static const char PLAYER_SYMBOL_HUMAN = 'X';
    static const char PLAYER_SYMBOL_BOT = 'O';
    static const char EMPTY_CELL_SYMBOL = '_';

    ConsoleGameView() {
    }

    void setGameModel(std::shared_ptr<GameModel> gameModel) {
        this->gameModel = gameModel;
        this->gameModel->addGameModelChangeHandler(shared_from_this());
    }

    void printField() {
        auto fieldSize = this->gameModel->getFieldSize();
        auto field = this->gameModel->getField();
        std::cout << "Current field state:" << std::endl;
        for (int row = 0; row < fieldSize; ++row) {
            for (int col = 0; col < fieldSize; ++col) {
                std::cout << this->convertCellValueToSymbol(field[row][col]);
            }
            std::cout << std::endl;
        }
    }

    void onGameModelError(std::string message) override {
        std::cout << "Error: " << message << std::endl;
    }

    void onGameModelChanged() override {
        switch (this->gameModel->getCurrentGameState()) {
            case GameModel::GAME_STATE_GAME_LOBBY:
                std::cout << "Welcome to the tac-tac-toe!" << std::endl;
                std::cout << "New session is going to start. Input 'stop' to interrupt the game." << std::endl;
                break;
            case GameModel::GAME_STATE_GAME_PARTY_CONFIGURATION_FIELD_SIZE :
                std::cout << "Input square field size [" << GameModel::FIELD_SQUARE_SIZE_DEFAULT << "]:" << std::endl;
                break;
            case GameModel::GAME_STATE_GAME_PARTY_CONFIGURATION_LINE_LENGTH :
                std::cout << "Input line target length [" << GameModel::LINE_TARGET_LENGTH_DEFAULT << "]:" << std::endl;
                break;
            case GameModel::GAME_STATE_GAME_PARTY_CONFIGURATION_WHO_IS_FIRST_PLAYER :
                std::cout << "How starts first? [player] Input 'bot' for the computer to start first." << std::endl;
                break;
            case GameModel::GAME_STATE_GAME_IN_PROGRESS_TURN_BOT :
                std::cout << "Computer's turn." << std::endl;
                break;
            case GameModel::GAME_STATE_GAME_IN_PROGRESS_TURN_HUMAN :
                std::cout << "Player's turn. Here is the current state of the field:" << std::endl;
                this->printField();
                std::cout << "Input target row and column separated with a space (for example, top right cell is 1 3 in 3x3 grid):" << std::endl;
                break;
            case GameModel::GAME_STATE_GAME_WINNER_BOT :
                std::cout << "Computer is the winner!" << std::endl;
                this->printField();
                break;
            case GameModel::GAME_STATE_GAME_WINNER_HUMAN :
                std::cout << "Player is the winner!" << std::endl;
                this->printField();
                break;
            case GameModel::GAME_STATE_GAME_STOPPED_BY_PLAYER :
                std::cout << "GameController interrupted by the player." << std::endl;
                break;
            default:
                std::cout << "Unexpected state" << std::endl;
                break;
        }
    }
};


#endif //NSU_OOP_2021_LABS_CONSOLEGAMEVIEW_H
