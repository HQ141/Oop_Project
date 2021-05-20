#pragma once
#include <string>

class WideString {
public:
    WideString(){}
    // version for wide strings
    WideString(const wchar_t* s) {
        // find length
        for (; s[len] != '\0'; len++)
            ;
        // allocate memory
        w_str = new wchar_t[len + 1];
        for (int i = 0; i < len; i++) {
            w_str[i] = s[i];
        }
        w_str[len] = '\0';
    }
    // version for narrow strings
    WideString(const char* s) {
        // find length
        for (; s[len] != '\0'; len++)
            ;
        // allocate memory
        w_str = new wchar_t[len + 1];
        for (int i = 0; i < len; i++) {
            w_str[i] = s[i];
        }
        w_str[len] = '\0';
    }
    // version for std::string
    WideString(std::string s)
        :
        WideString(s.c_str())
    {
    }
    // destructor
    ~WideString() {
        if (w_str != NULL) {
            delete[] w_str;
            w_str = NULL;
            len = 0;
        }
    }
    // copy constructor
    WideString(const WideString& s)
    {
        *this = s;
    }
    // assignment operator
    WideString& operator=(const WideString& s) {
        // clear old buffer if any
        if (w_str != NULL) {
            delete[] w_str;
            w_str = NULL;
        }
        // get new length
        len = s.length();
        // allocate memory
        w_str = new wchar_t[len+1];
        // get wchar_t array
        const wchar_t* temp = s.cw_str();

        for (int i = 0; i < len; i++) {
            w_str[i] = temp[i];
        }
        w_str[len] = '\0';
    }

    int length() const {
        return len;
    }
    const wchar_t* cw_str() const {
        return w_str;
    }
    std::string str() {
        std::string str;
        for (int i = 0; i < len; i++) {
            str += (char)w_str[i];
        }
        return str;
    }
private:
    wchar_t* w_str = NULL;
    long int len = 0;
};