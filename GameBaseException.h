//
// Created by alstein on 09/12/2021.
//

#ifndef NSU_OOP_2021_LABS_GAMEBASEEXCEPTION_H
#define NSU_OOP_2021_LABS_GAMEBASEEXCEPTION_H

#include <exception>
#include <string>

class GameBaseException: public std::exception {
protected:
    std::string message;
public:
    GameBaseException() {
    }

    GameBaseException(const char *message) {
        this->message = message;
    }

    virtual const char *what() const throw() {
        return message.c_str();
    }
};


#endif //NSU_OOP_2021_LABS_GAMEBASEEXCEPTION_H
