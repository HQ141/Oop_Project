#ifndef GAMEERROR_H  
#define GAMEERROR_H
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <exception>

namespace gameErrorNS{
    
    const int FATAL_ERROR = -1;
    const int WARNING = 1;
}


class GameError : public std::exception{
private:
    int     errorCode;
    std::string message;
    LPCWSTR w_message;
public:
    
    GameError() throw() :errorCode(gameErrorNS::FATAL_ERROR), message("Undefined Error in game.") {}
    GameError(const GameError& e) throw(): std::exception(e), errorCode(e.errorCode), message(e.message) {}
    GameError(int code, const std::string &s) throw() :errorCode(code), message(s) {}
    GameError(int code, const LPCWSTR s) throw() : errorCode(code), w_message(s) {}
    GameError& operator= (const GameError& rhs) throw() {
        std::exception::operator=(rhs);
        this->errorCode = rhs.errorCode;
        this->message = rhs.message;
    }
    virtual ~GameError() noexcept {};
    virtual const char* what() const throw() { return this->getMessage(); }
    virtual LPCWSTR w_what() const throw() { return this->w_getMessage(); }
    const char* getMessage() const throw() { return message.c_str(); }
    const LPCWSTR w_getMessage() const throw() { return w_message; }
    int getErrorCode() const throw() { return errorCode; }
};

#endif
