#ifndef NSU_OOP_2021_LABS_GAMEPARTICIPANTHUMAN_H
#define NSU_OOP_2021_LABS_GAMEPARTICIPANTHUMAN_H

#include "GameParticipant.h"
#include "GameBaseException.h"
#include <sstream>
#include <utility>

class GameParticipantHuman : public GameParticipant {
public:
    static const constexpr char *GAME_PARTICIPANT_CODE = "human";

    std::pair<unsigned int, unsigned int>
    chooseCell(std::string input, int fieldSize, int **field, int emptyCellMarker, int thisPlayerMarker) override {
        unsigned rowPlusOne;
        unsigned colPlusOne;
        std::stringstream stringstream(input);

        stringstream >> rowPlusOne >> colPlusOne;
        if (input.empty() || stringstream.fail()) {
            throw GameBaseException("Input unrecognized. Please retry.");
        } else {
            return std::pair<unsigned, unsigned>(rowPlusOne - 1, colPlusOne - 1);
        }
    }

    std::string indicationMessage() override {
        return "Input target row and column separated with a space (for example, top right cell is 1 3 in 3x3 grid).";
    }

    bool isExpectingInput() override {
        return true;
    }
};


#endif //NSU_OOP_2021_LABS_GAMEPARTICIPANTHUMAN_H
