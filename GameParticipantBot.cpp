#include "GameParticipantBot.h"
#include "GameParticipantCreatorConcrete.h"

static GameParticipantCreatorConcrete<GameParticipantBot> creator(GameParticipantBot::GAME_PARTICIPANT_CODE);