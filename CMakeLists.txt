cmake_minimum_required(VERSION 3.17)
project(OOP_Exercise_4)

set(CMAKE_CXX_STANDARD 14)

add_executable(OOP_Exercise_4
        main.cpp
        GameController.h
        GameModel.h
        GameModel.cpp
        GameModelChangeHandler.cpp
        GameModelChangeHandler.h
        GameParticipant.cpp
        GameParticipant.h
        GameParticipantBot.cpp
        GameParticipantBot.h
        GameParticipantHuman.h
        GameParticipantHuman.cpp
        GameBaseException.cpp
        GameBaseException.h
        GameController.cpp
        ConsoleGameView.h
        ConsoleGameView.cpp
        GameParticipantFactory.cpp
        GameParticipantFactory.h
        GameParticipantCreator.cpp
        GameParticipantCreator.h
        GameParticipantCreatorConcrete.cpp
        GameParticipantCreatorConcrete.h)        
