#ifndef NSU_OOP_2021_LABS_GAMEPARTICIPANTBOT_H
#define NSU_OOP_2021_LABS_GAMEPARTICIPANTBOT_H

#include <string>
#include "GameParticipant.h"
#include "GameBaseException.h"

class GameParticipantBot : public GameParticipant {
public:
    static const constexpr char *GAME_PARTICIPANT_CODE = "bot";

    std::pair<unsigned , unsigned > chooseCell(std::string input, int fieldSize, int **field, int emptyCellMarker, int thisPlayerMarker) override {
        // we are choosing the first empty cell
        // a better algorithm can exist :)
        for (unsigned row = 0; row < fieldSize; ++row) {
            for (unsigned col = 0; col < fieldSize; ++col) {
                if (field[row][col] == emptyCellMarker) {
                    return std::pair<unsigned, unsigned>(row, col);
                }
            }
        }

        throw GameBaseException("Failed to choose a cell for a move");
    }

    std::string indicationMessage() override {
        return "Prepare to get impressed with the artificial intelligence! :-)";
    }

    bool isExpectingInput() override {
        return false;
    }
};


#endif //NSU_OOP_2021_LABS_GAMEPARTICIPANTBOT_H
