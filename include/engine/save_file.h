#ifndef SAVE_FILE_H
#define SAVE_FILE_H

#include <string>

class SaveFile{
public:

    static void Initialize(std::string developerName, std::string gameName);

    static void write(std::string fileName, std::string data);
    static std::string read(std::string fileName);
    static void remove(std::string fileName);
    static bool exists(std::string fileName);
    static void clear();

    static std::string getSaveFolder();
private:
    // Define Developer
    // Define Game

    static std::string saveFolder;
    static std::string developerName;
    static std::string gameName;
};

#endif // SAVE_FILE_H