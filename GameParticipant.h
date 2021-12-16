#ifndef NSU_OOP_2021_LABS_GAMEPARTICIPANT_H
#define NSU_OOP_2021_LABS_GAMEPARTICIPANT_H

#include <string>
#include <utility>

class GameParticipant {
public:
    virtual std::pair<unsigned, unsigned> chooseCell(std::string input, int fieldSize, int **field, int emptyCellMarker, int thisPlayerMarker) = 0;
    virtual bool isExpectingInput() = 0;
    virtual std::string indicationMessage() = 0;
};


#endif //NSU_OOP_2021_LABS_GAMEPARTICIPANT_H
