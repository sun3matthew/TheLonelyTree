#include <engine/str_utils.h>

std::vector<std::string> StrUtils::split(std::string& str, char delim){
    std::vector<std::string> splitName;
    std::string temp = "";
    for (char c : str){
        if (c == delim){
            splitName.push_back(temp);
            temp = "";
        } else {
            temp += c;
        }
    }
    if(temp != "")
        splitName.push_back(temp);
    return splitName;
}

std::string StrUtils::join(std::vector<std::string>& vec, char delim){
    std::string temp = "";
    for (std::string str : vec){
        temp += str + delim;
    }
    return temp;
}

std::string StrUtils::toUpper(std::string& str){
    std::string temp = "";
    for (char c : str){
        if (isalnum(c)){
            temp += toupper(c);
        }
    }
    return temp;
}

std::string StrUtils::toLower(std::string& str){
    std::string temp = "";
    for (char c : str){
        if (isalnum(c)){
            temp += tolower(c);
        }
    }
    return temp;
}