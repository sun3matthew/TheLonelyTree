#include <engine/save_file.h>
#include <engine/platform_folders.h>

#include <engine/logger.h>

#include <fstream>
#include <filesystem>

std::string SaveFile::saveFolder = "";
std::string SaveFile::developerName = "";
std::string SaveFile::gameName = "";

void SaveFile::Initialize(std::string developerName, std::string gameName){
    SaveFile::developerName = developerName;
    SaveFile::gameName = gameName;

    saveFolder = sago::getSaveGamesFolder1() + "/" + developerName + "/" + gameName;

    if (!std::filesystem::exists(saveFolder)){
        std::filesystem::create_directories(saveFolder);
    }

    Logger::log("Save Folder: " + saveFolder);
}

void SaveFile::write(std::string fileName, std::string data){
    std::string filePath = saveFolder + "/" + fileName;
    std::ofstream file(filePath);
    file << data;
    file.close();
}

std::string SaveFile::read(std::string fileName){
    if (saveFolder == "" || !exists(fileName)){
        return "";
    }
    std::string filePath = saveFolder + "/" + fileName;
    std::ifstream file(filePath);
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return data;
}

void SaveFile::remove(std::string fileName){
    if (saveFolder == ""){
        return;
    }
    std::string filePath = saveFolder + "/" + fileName;
    std::remove(filePath.c_str());
}

bool SaveFile::exists(std::string fileName){
    std::string filePath = saveFolder + "/" + fileName;
    std::ifstream file(filePath);
    return file.good();
}

void SaveFile::clear(){
    if (saveFolder == ""){
        return;
    }
    std::string filePath = saveFolder;
    std::remove(filePath.c_str());
}

std::string SaveFile::getSaveFolder(){
    return saveFolder;
}

std::string SaveFile::createLogFile(){
    int count = 0;
    std::string logDir = saveFolder + "/logs";
    if (!std::filesystem::exists(logDir)){
        std::filesystem::create_directories(logDir);
    }
    for (const auto& entry : std::filesystem::directory_iterator(logDir)){
        if (entry.path().extension() == ".txt"){
            count++;
        }
    }

    std::string logFile = logDir + "/log-" + std::to_string(count) + ".txt";
    write(logFile, "");
    return logFile;
}