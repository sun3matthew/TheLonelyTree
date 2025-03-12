#ifndef SAVE_FILE_H
#define SAVE_FILE_H

#include <string>

class SaveFile{
public:

    static void Initialize(std::string developerName, std::string gameName);

    static void Save(std::string fileName, std::string data);
    static std::string Load(std::string fileName);
    static void Delete(std::string fileName);
    static bool Exists(std::string fileName);
    static void Clear();

    static std::string getSaveFolder();
private:
    // Define Developer
    // Define Game

    static std::string saveFolder;
    static std::string developerName;
    static std::string gameName;
};

#endif // SAVE_FILE_H