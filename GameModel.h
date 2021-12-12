#ifndef NSU_OOP_2021_LABS_GAMEMODEL_H
#define NSU_OOP_2021_LABS_GAMEMODEL_H

#include <list>
#include <memory>
#include <exception>
#include <sstream>
#include <iostream>
#include "GameParticipant.h"
#include "GameModelChangeHandler.h"
#include "GameBaseException.h"

class GameModel {
protected:
    int currentGameState;
    unsigned fieldSize;
    unsigned lineTargetLength;
    // value is CELL_VALUE_EMPTY_CELL for empty cells and participant id otherwise
    std::list<std::shared_ptr<GameModelChangeHandler>> changeHandlers;
    int **field;

    void notifyHandlersStateChange() {
        for (auto it = this->changeHandlers.begin(); it != this->changeHandlers.end(); ++it) {
            (*it)->onGameModelChanged();
        }
    }

    void notifyHandlersError(std::string message) {
        for (auto it = this->changeHandlers.begin(); it != this->changeHandlers.end(); ++it) {
            (*it)->onGameModelError(message);
        }
    }

    void initializeField(const unsigned fieldSize) {
        this->fieldSize = fieldSize;
        field = new int *[fieldSize];
        for (int row = 0; row < fieldSize; ++row) {
            field[row] = new int[fieldSize];
        }

        // init field with empty data
        for (int row = 0; row < fieldSize; ++row) {
            for (int col = 0; col < fieldSize; ++col) {
                field[row][col] = CELL_VALUE_EMPTY_CELL;
            }
        }
    }

    bool detectWinner(unsigned moveRow, unsigned moveCol) {
        int cellValue = this->field[moveRow][moveCol];
        // we check in every direction if the targetLineLength of the same
        // cell values are present

        // horizontal check
        unsigned horizontalScore = 1;
        for (int col = moveCol + 1; col < this->fieldSize; ++col) {
            if (field[moveRow][col] == cellValue) {
                horizontalScore++;
            } else {
                break;
            }
        }
        for (int col = moveCol - 1; col >= 0; --col) {
            if (field[moveRow][col] == cellValue) {
                horizontalScore++;
            } else {
                break;
            }
        }

        if (horizontalScore >= this->lineTargetLength) {
            return true;
        }

        // vertical check
        unsigned verticalScore = 1;
        for (int row = moveRow + 1; row < this->fieldSize; ++row) {
            if (field[row][moveCol] == cellValue) {
                verticalScore++;
            } else {
                break;
            }
        }
        for (int row = moveRow - 1; row >= 0; --row) {
            if (field[row][moveCol] == cellValue) {
                verticalScore++;
            } else {
                break;
            }
        }

        if (verticalScore >= this->lineTargetLength) {
            return true;
        }

        // diagonal check
        unsigned diagonalScore = 1;
        for (int row = moveRow + 1, col = moveCol + 1;
             row < this->fieldSize && col < this->fieldSize;
             ++row, ++col) {
            if (field[row][col] == cellValue) {
                diagonalScore++;
            } else {
                break;
            }
        }

        for (int row = moveRow - 1, col = moveCol - 1; row >= 0 && col >= 0; --row, --col) {
            if (field[row][col] == cellValue) {
                diagonalScore++;
            } else {
                break;
            }
        }

        if (diagonalScore >= this->lineTargetLength) {
            return true;
        }

        // anti-diagonal check
        unsigned antiDiagonalScore = 1;
        for (int row = moveRow + 1, col = moveCol - 1; row < this->fieldSize && col >= 0; ++row, --col) {
            if (this->field[row][col] == cellValue) {
                antiDiagonalScore++;
            } else {
                break;
            }
        }

        for (int row = moveRow - 1, col = moveCol + 1; row >= 0 && col < this->fieldSize; --row, ++col) {
            if (this->field[row][col] == cellValue) {
                antiDiagonalScore++;
            } else {
                break;
            }
        }
        if (antiDiagonalScore >= this->lineTargetLength) {
            return true;
        }

        return false;
    }


    void changeState(const int targetGameState) {
        this->currentGameState = targetGameState;
        this->notifyHandlersStateChange();
    }

    void declareError(std::string message) {
        this->notifyHandlersError(message);
    }
public:
    static const unsigned FIELD_SQUARE_SIZE_DEFAULT = 3;
    static const unsigned LINE_TARGET_LENGTH_DEFAULT = 3;

    static const int CELL_VALUE_EMPTY_CELL = -1;
    static const int CELL_VALUE_HUMAN_CELL = 1;
    static const int CELL_VALUE_BOT_CELL = 2;

    // do we want to start a new game party?
    static const int GAME_STATE_GAME_LOBBY = 0;
    // getting the field size
    static const int GAME_STATE_GAME_PARTY_CONFIGURATION_FIELD_SIZE = 1;
    // getting the target line length
    static const int GAME_STATE_GAME_PARTY_CONFIGURATION_LINE_LENGTH = 2;
    // getting the first participant
    static const int GAME_STATE_GAME_PARTY_CONFIGURATION_WHO_IS_FIRST_PLAYER = 3;
    static const int GAME_STATE_GAME_IN_PROGRESS_TURN_BOT = 4;
    static const int GAME_STATE_GAME_IN_PROGRESS_TURN_HUMAN = 5;
    static const int GAME_STATE_GAME_WINNER_HUMAN = 6;
    static const int GAME_STATE_GAME_WINNER_BOT = 7;
    static const int GAME_STATE_GAME_STOPPED_BY_PLAYER = 8;

    GameModel() {
        this->currentGameState = GAME_STATE_GAME_LOBBY;
        this->field = nullptr;
        this->fieldSize = 0;
        this->lineTargetLength = 0;
    }

    virtual ~GameModel() {
        this->clear();
    }

    void processInput(std::string &input) {
        std::stringstream stringstream(input);
        switch (this->currentGameState) {
            case GAME_STATE_GAME_LOBBY: {
                if (input == "stop") {
                    this->changeState(GAME_STATE_GAME_STOPPED_BY_PLAYER);
                } else {
                    this->changeState(GAME_STATE_GAME_PARTY_CONFIGURATION_FIELD_SIZE);
                }
                break;
            }
            case GAME_STATE_GAME_PARTY_CONFIGURATION_FIELD_SIZE : {
                unsigned fieldSquareSize;
                stringstream >> fieldSquareSize;
                if (input.empty() || stringstream.fail()) {
                    fieldSquareSize = FIELD_SQUARE_SIZE_DEFAULT;
                }
                this->fieldSize = fieldSquareSize;
                this->initializeField(this->fieldSize);
                this->changeState(GAME_STATE_GAME_PARTY_CONFIGURATION_LINE_LENGTH);
                break;
            }
            case GAME_STATE_GAME_PARTY_CONFIGURATION_LINE_LENGTH : {
                unsigned lineTargetLength;
                stringstream >> lineTargetLength;
                if (input.empty() || stringstream.fail()) {
                    lineTargetLength = LINE_TARGET_LENGTH_DEFAULT;
                }
                this->lineTargetLength = lineTargetLength;
                this->changeState(GAME_STATE_GAME_PARTY_CONFIGURATION_WHO_IS_FIRST_PLAYER);
                break;
            }
            case GAME_STATE_GAME_PARTY_CONFIGURATION_WHO_IS_FIRST_PLAYER : {
                if (input != "bot") {
                    this->changeState(GAME_STATE_GAME_IN_PROGRESS_TURN_HUMAN);
                } else {
                    this->changeState(GAME_STATE_GAME_IN_PROGRESS_TURN_BOT);
                }
                break;
            }
            case GAME_STATE_GAME_IN_PROGRESS_TURN_BOT : {
                // we are choosing the first empty cell
                // a better algorithm can exist :)
                bool movePerformed = false;
                unsigned moveRow;
                unsigned moveCol;
                for (unsigned row = 0; row < this->fieldSize && !movePerformed; ++row) {
                    for (unsigned col = 0; col < this->fieldSize && !movePerformed; ++col) {
                        if (this->field[row][col] == CELL_VALUE_EMPTY_CELL) {
                            moveRow = row;
                            moveCol = col;
                            this->field[moveRow][moveCol] = CELL_VALUE_BOT_CELL;
                            movePerformed = true;
                        }
                    }
                }

                bool botWinner = this->detectWinner(moveRow, moveCol);
                if (botWinner) {
                    this->changeState(GAME_STATE_GAME_WINNER_BOT);
                } else {
                    this->changeState(GAME_STATE_GAME_IN_PROGRESS_TURN_HUMAN);
                }
                break;
            }
            case GAME_STATE_GAME_IN_PROGRESS_TURN_HUMAN : {
                unsigned rowPlusOne;
                unsigned colPlusOne;
                stringstream >> rowPlusOne >> colPlusOne;
                if (input.empty() || stringstream.fail()) {
                    this->declareError("Input unrecognized. Please retry.");
                } else if (rowPlusOne > this->fieldSize || colPlusOne > this->fieldSize) {
                    this->declareError("Incorrect row or column. Please retry.");
                } else if (this->field[rowPlusOne - 1][colPlusOne - 1] != CELL_VALUE_EMPTY_CELL) {
                    this->declareError("Cell is not empty. Please retry.");
                } else {
                    this->field[rowPlusOne - 1][colPlusOne - 1] = CELL_VALUE_HUMAN_CELL;
                    bool humanWinner = this->detectWinner(rowPlusOne - 1, colPlusOne - 1);
                    if (humanWinner) {
                        this->changeState(GAME_STATE_GAME_WINNER_HUMAN);
                    } else {
                        this->changeState(GAME_STATE_GAME_IN_PROGRESS_TURN_BOT);
                    }
                }
                break;
            }
            case GAME_STATE_GAME_WINNER_BOT : {
                this->changeState(GAME_STATE_GAME_LOBBY);
                break;
            }
            case GAME_STATE_GAME_WINNER_HUMAN : {
                this->changeState(GAME_STATE_GAME_LOBBY);
                break;
            }
            default: {
                throw GameBaseException("Unreachable section");
                break;
            }
        }
    }

    void clear() {
        if (field != nullptr) {
            for (int row = 0; row < this->fieldSize; ++row) {
                delete[] field[row];
            }
            delete[] field;
            field = nullptr;
        }
        this->fieldSize = 0;
        this->lineTargetLength = 0;
    }

    unsigned getFieldSize() const {
        return this->fieldSize;
    };

    int **getField() const {
        return field;
    }

    int getCurrentGameState() const {
        return this->currentGameState;
    }

    void addGameModelChangeHandler(std::shared_ptr<GameModelChangeHandler> handler) {
        this->changeHandlers.push_back(handler);
        handler->onGameModelChanged();
    }

    bool isFinished() const {
        return this->currentGameState == GAME_STATE_GAME_WINNER_HUMAN
               || this->currentGameState == GAME_STATE_GAME_WINNER_BOT
               || this->currentGameState == GAME_STATE_GAME_STOPPED_BY_PLAYER;
    }

    bool isContinueWithAnotherParty() const {
        return this->currentGameState == GAME_STATE_GAME_WINNER_BOT
               || this->currentGameState == GAME_STATE_GAME_WINNER_HUMAN;
    }

    bool isExpectingInput() const {
        return this->currentGameState != GAME_STATE_GAME_IN_PROGRESS_TURN_BOT;
    }
};


#endif //NSU_OOP_2021_LABS_GAMEMODEL_H
