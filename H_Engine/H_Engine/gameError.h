#ifndef GAMEERROR_H  
#define GAMEERROR_H
#define WIN32_LEAN_AND_MEAN

#include <exception>
#include "WideString.h"
#include <sstream>

namespace gameErrorNS{
    
    const int FATAL_ERROR = -1;
    const int WARNING = 1;
}

// this is based almost entirely on the ChiliException class 
// made by a youtuber with the name 'planetchili'

class GameError : public std::exception {
public:
    GameError(int code, const char* desc, const char* filename, int line) noexcept 
        :
        code(code),
        line(line),
        desc(desc),
        filename(filename)
    {
    }
    GameError(int code, std::string desc, const char* filename, int line) noexcept
        :
        code(code),
        line(line),
        desc(desc),
        filename(filename)
    {
    }
    GameError(int code, const char* filename, int line) noexcept
        :
        code(code),
        line(line),
        desc(""),
        filename(filename)
    {
    }
    virtual ~GameError() noexcept 
    {
    }

    virtual const wchar_t* w_what() const noexcept
    {
        return WideString(what()).cw_str();
    }

    virtual const char* what() const noexcept override
    {
        std::ostringstream oss;
        oss << "[Code] " << code << std::endl
            << "[Type] " << GetType() << std::endl
            << GetOriginString();
        if (desc.size() > 0) {
            oss << std::endl
                << "[Description] " << GetErrorString();
        }
        whatBuffer = oss.str();
        return whatBuffer.c_str();
    }

    virtual std::string GetType() const
    {
        return "Game Error";
    }

    std::string GetOriginString() const {
        std::ostringstream oss;
        oss << "[File] " << filename << std::endl
            << "[Line] " << line;
        return oss.str();
    }

    virtual std::string GetErrorString() const {
        return desc;
    }

private:
    int code;
    int line;
    std::string filename;
    std::string desc;
protected:
    mutable std::string whatBuffer;
};

#define ERROR_DESC(code, desc) GameError(code, desc, __FILE__, __LINE__)
#define ERROR_NO_DESC(code) GameError(code, __FILE__, __LINE__)

#endif
