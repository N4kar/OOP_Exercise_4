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
            case GameModel::CELL_VALUE_FIRST_PARTICIPANT:
                return PLAYER_SYMBOL_FIRST_PARTICIPANT;
            case GameModel::CELL_VALUE_SECOND_PARTICIPANT:
                return PLAYER_SYMBOL_SECOND_PARTICIPANT;
            default:
                throw GameBaseException("Unreachable section");
        }
    }

public:
    static const char PLAYER_SYMBOL_FIRST_PARTICIPANT = 'X';
    static const char PLAYER_SYMBOL_SECOND_PARTICIPANT = 'O';
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
            case GameModel::GAME_STATE_GAME_PARTY_CONFIGURATION_FIRST_PARTICIPANT :
                std::cout << "Who is the first player? [bot] Input 'bot' for the computer and 'human' for the human."
                          << std::endl;
                break;
            case GameModel::GAME_STATE_GAME_PARTY_CONFIGURATION_SECOND_PARTICIPANT :
                std::cout << "What is the second player? [bot] Input 'bot' for the computer and 'human' for the human."
                          << std::endl;
                break;
            case GameModel::GAME_STATE_GAME_TURN_FIRST_PARTICIPANT :
                std::cout << "First player's turn (playing with " << PLAYER_SYMBOL_FIRST_PARTICIPANT
                          << "). Here is the current state of the field:" << std::endl;
                this->printField();
                break;
            case GameModel::GAME_STATE_GAME_TURN_SECOND_PARTICIPANT :
                std::cout << "Second player's turn (playing with " << PLAYER_SYMBOL_SECOND_PARTICIPANT
                          << "). Here is the current state of the field:" << std::endl;
                this->printField();
                break;
            case GameModel::GAME_STATE_GAME_WINNER_FIRST_PARTICIPANT :
                std::cout << "First player (playing with " << PLAYER_SYMBOL_FIRST_PARTICIPANT << ") is the winner!"
                          << std::endl;
                this->printField();
                break;
            case GameModel::GAME_STATE_GAME_WINNER_SECOND_PARTICIPANT :
                std::cout << "Second player (playing with " << PLAYER_SYMBOL_SECOND_PARTICIPANT << ") is the winner!"
                          << std::endl;
                this->printField();
                break;
            case GameModel::GAME_STATE_GAME_TIE :
                std::cout << "It is a tie for this time!" << std::endl;
                this->printField();
                break;
            case GameModel::GAME_STATE_GAME_STOPPED_BY_PLAYER :
                std::cout << "Game stopped by the player." << std::endl;
                break;
            default:
                std::cout << "Unexpected state" << std::endl;
                break;
        }

        if (!this->gameModel->getIndicationMessage().empty()) {
            std::cout << "Indication: " << this->gameModel->getIndicationMessage() << std::endl;
        }
    }
};


#endif //NSU_OOP_2021_LABS_CONSOLEGAMEVIEW_H
