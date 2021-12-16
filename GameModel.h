#ifndef NSU_OOP_2021_LABS_GAMEMODEL_H
#define NSU_OOP_2021_LABS_GAMEMODEL_H

#include <list>
#include <memory>
#include <exception>
#include <sstream>
#include <iostream>
#include "GameParticipant.h"
#include "GameParticipantBot.h"
#include "GameModelChangeHandler.h"
#include "GameParticipantFactory.h"
#include "GameBaseException.h"

class GameModel {
protected:
    int currentGameState;
    unsigned fieldSize;
    unsigned lineTargetLength;
    std::shared_ptr<GameParticipant> firstParticipant;
    std::shared_ptr<GameParticipant> secondParticipant;
    // value is CELL_VALUE_EMPTY_CELL for empty cells and participant id otherwise
    std::list<std::shared_ptr<GameModelChangeHandler>> changeHandlers;
    int **field;
    std::string indicationMessage;
    int emptyCellsCount;

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

        this->emptyCellsCount = fieldSize * fieldSize;
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
        switch (this->currentGameState) {
            case GAME_STATE_GAME_TURN_FIRST_PARTICIPANT:
                this->indicationMessage = this->firstParticipant->indicationMessage();
                break;
                case GAME_STATE_GAME_TURN_SECOND_PARTICIPANT:
                this->indicationMessage = this->secondParticipant->indicationMessage();
                break;
            default:
                this->indicationMessage = "";
        }

        this->notifyHandlersStateChange();
    }

    void declareError(std::string message) {
        this->notifyHandlersError(message);
    }

    // processing first or second participant's move in generic way
    void processParticipantMove(
            std::shared_ptr<GameParticipant> participant,
            int participantCellValue,
            std::string input,
            int winnerState,
            int nextMoveState
    ) {
        try {
            std::pair<unsigned, unsigned> rowCol = participant->chooseCell(input, this->fieldSize, this->field, CELL_VALUE_EMPTY_CELL, participantCellValue);

            if (rowCol.first >= this->fieldSize || rowCol.second >= this->fieldSize) {
                this->declareError("Incorrect row or column. Please retry.");
            } else if (this->field[rowCol.first][rowCol.second] != CELL_VALUE_EMPTY_CELL) {
                this->declareError("Cell is not empty. Please retry.");
            } else {
                this->field[rowCol.first][rowCol.second] = participantCellValue;
                this->emptyCellsCount--;
                if (this->detectWinner(rowCol.first, rowCol.second)) {
                    this->changeState(winnerState);
                } else {
                    if (this->emptyCellsCount > 0) {
                        this->changeState(nextMoveState);
                    } else {
                        this->changeState(GAME_STATE_GAME_TIE);
                    }
                }
            }
        } catch (GameBaseException &exception) {
            this->declareError(exception.what());
        }
    }

public:
    static const constexpr char *GAME_PARTICIPANT_CODE_DEFAULT = GameParticipantBot::GAME_PARTICIPANT_CODE;
    static const unsigned FIELD_SQUARE_SIZE_DEFAULT = 3;
    static const unsigned LINE_TARGET_LENGTH_DEFAULT = 3;

    static const int CELL_VALUE_EMPTY_CELL = -1;
    static const int CELL_VALUE_SECOND_PARTICIPANT = 1;
    static const int CELL_VALUE_FIRST_PARTICIPANT = 2;

    // do we want to start a new game party?
    static const int GAME_STATE_GAME_LOBBY = 0;
    // getting the field size
    static const int GAME_STATE_GAME_PARTY_CONFIGURATION_FIELD_SIZE = 1;
    // getting the target line length
    static const int GAME_STATE_GAME_PARTY_CONFIGURATION_LINE_LENGTH = 2;
    // getting the first participant
    static const int GAME_STATE_GAME_PARTY_CONFIGURATION_FIRST_PARTICIPANT = 3;
    static const int GAME_STATE_GAME_PARTY_CONFIGURATION_SECOND_PARTICIPANT = 4;
    static const int GAME_STATE_GAME_TURN_FIRST_PARTICIPANT = 5;
    static const int GAME_STATE_GAME_TURN_SECOND_PARTICIPANT = 6;
    static const int GAME_STATE_GAME_WINNER_SECOND_PARTICIPANT = 7;
    static const int GAME_STATE_GAME_WINNER_FIRST_PARTICIPANT = 8;
    static const int GAME_STATE_GAME_TIE = 9;
    static const int GAME_STATE_GAME_STOPPED_BY_PLAYER = 10;

    GameModel() {
        this->currentGameState = GAME_STATE_GAME_LOBBY;
        this->field = nullptr;
        this->emptyCellsCount = 0;
        this->fieldSize = 0;
        this->lineTargetLength = 0;
    }

    virtual ~GameModel() {
        this->clear();
    }

    void processInput(std::string &input) {
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
                std::stringstream stringstream(input);
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
                std::stringstream stringstream(input);
                unsigned lineTargetLength;
                stringstream >> lineTargetLength;
                if (input.empty() || stringstream.fail()) {
                    lineTargetLength = LINE_TARGET_LENGTH_DEFAULT;
                }
                this->lineTargetLength = lineTargetLength;
                this->changeState(GAME_STATE_GAME_PARTY_CONFIGURATION_FIRST_PARTICIPANT);
                break;
            }
            case GAME_STATE_GAME_PARTY_CONFIGURATION_FIRST_PARTICIPANT : {
                try {
                    this->firstParticipant = GameParticipantFactory::getInstance().create(input);
                } catch (GameBaseException &exception) {
                    this->firstParticipant = GameParticipantFactory::getInstance().create(
                            GAME_PARTICIPANT_CODE_DEFAULT);
                }
                this->changeState(GAME_STATE_GAME_PARTY_CONFIGURATION_SECOND_PARTICIPANT);
                break;
            }
            case GAME_STATE_GAME_PARTY_CONFIGURATION_SECOND_PARTICIPANT : {
                try {
                    this->secondParticipant = GameParticipantFactory::getInstance().create(input);
                } catch (GameBaseException &exception) {
                    this->secondParticipant = GameParticipantFactory::getInstance().create(
                            GAME_PARTICIPANT_CODE_DEFAULT);
                }
                this->changeState(GAME_STATE_GAME_TURN_FIRST_PARTICIPANT);
                break;
            }
            case GAME_STATE_GAME_TURN_FIRST_PARTICIPANT : {
                this->processParticipantMove(this->firstParticipant, CELL_VALUE_FIRST_PARTICIPANT, input,
                                             GAME_STATE_GAME_WINNER_FIRST_PARTICIPANT,
                                             GAME_STATE_GAME_TURN_SECOND_PARTICIPANT);

                break;
            }
            case GAME_STATE_GAME_TURN_SECOND_PARTICIPANT : {
                this->processParticipantMove(this->secondParticipant, CELL_VALUE_SECOND_PARTICIPANT, input,
                                             GAME_STATE_GAME_WINNER_SECOND_PARTICIPANT,
                                             GAME_STATE_GAME_TURN_FIRST_PARTICIPANT);
                break;
            }
            case GAME_STATE_GAME_WINNER_FIRST_PARTICIPANT : {
                this->changeState(GAME_STATE_GAME_LOBBY);
                break;
            }
            case GAME_STATE_GAME_WINNER_SECOND_PARTICIPANT : {
                this->changeState(GAME_STATE_GAME_LOBBY);
                break;
            }
            case GAME_STATE_GAME_TIE: {
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
        this->emptyCellsCount = 0;
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
        return this->currentGameState == GAME_STATE_GAME_WINNER_SECOND_PARTICIPANT
               || this->currentGameState == GAME_STATE_GAME_WINNER_FIRST_PARTICIPANT
               || this->currentGameState == GAME_STATE_GAME_STOPPED_BY_PLAYER
               || this->currentGameState == GAME_STATE_GAME_TIE;
    }

    bool isContinueWithAnotherParty() const {
        return this->currentGameState == GAME_STATE_GAME_WINNER_FIRST_PARTICIPANT
               || this->currentGameState == GAME_STATE_GAME_WINNER_SECOND_PARTICIPANT
               || this->currentGameState == GAME_STATE_GAME_TIE;
    }

    bool isExpectingInput() const {
        // input is always expected except if the participant does not need it (the bot case)
        switch (this->currentGameState) {
            case GAME_STATE_GAME_TURN_FIRST_PARTICIPANT:
                return this->firstParticipant->isExpectingInput();
            case GAME_STATE_GAME_TURN_SECOND_PARTICIPANT:
                return this->secondParticipant->isExpectingInput();
            default:
                return true;
        }
    }

    std::string &getIndicationMessage() {
        return this->indicationMessage;
    }
};


#endif //NSU_OOP_2021_LABS_GAMEMODEL_H
