#pragma once

#include <vector>
#include <string>

class StrUtils{
    public:
        static std::vector<std::string> split(std::string& str, char delim);
        static std::string join(std::vector<std::string>& vec, char delim);
        static std::string toUpper(std::string& str);
        static std::string toLower(std::string& str);
};  
