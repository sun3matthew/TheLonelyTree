#include "entry_type.h"
#include "entry.h"
#include "user_tree.h"

#include <engine/str_utils.h>

#include <algorithm>
#include <unordered_set>
#include <string>

const std::unordered_set<std::string> Entry::commonWords = {
    "the", "is", "in", "and", "of", "to", "a", "with", "for", "on", "at", "by", "from", "an", "as",
    "be", "this", "that", "it", "or", "but", "not", "are", "were", "will", "would", "shall", "can", 
    "all", "any", "each", "every", "some", "so", "up", "down", "into", "out", "about", "over", 
    "under", "again", "further", "then", "once", "here", "there", "how", "why", "when", "where", 
    "who", "whom", "which", "whose", "you", "we", "they", "i", "me", "us", "he", "him", "she", 
    "her", "it", "they", "them", "my", "your", "his", "her", "its", "our", "their", "ours", "yours"
};

// Basic
std::string removePlural(const std::string& word) {
    if (word.size() > 2 && word.substr(word.size() - 2) == "es") {
        return word.substr(0, word.size() - 2);
    }
    else if (word.size() > 1 && word.back() == 's') {
        return word.substr(0, word.size() - 1);
    }
    return word;
}

Entry::Entry(unsigned long long branchID, std::string date, std::string name, std::string entry){
    std::replace(date.begin(), date.end(), '\n', ' ');
    std::replace(name.begin(), name.end(), '\n', ' ');

    this->branchID = branchID;
    this->commitID = 0;

    this->date = date;
    this->name = name;
    this->entry = entry;

    this->data = std::to_string(branchID) + "\n" + date + "\n" + name + "\n" + entry;

    recalculateKey();
}

Entry::Entry(std::string key, std::string data){
    this->commitID = std::stoi(StrUtils::split(key, '-')[1]); // TODO test to see if this works with uint

    //TODO
    std::vector<std::string> splitData = StrUtils::split(data, '\n');
    this->branchID = std::stoull(splitData[0]);
    this->date = splitData[1];
    this->name = splitData[2];
    this->entry = splitData[3];

    this->data = data;

    recalculateKey();
}

Entry::~Entry(){
}

void Entry::recalculateKey(){
    // * Simplifies key to it's base parts, merges "similar entries"
    // split name by spaces, remove common words, remove special characters
    std::vector<std::string> splitName = StrUtils::split(name, ',');
    std::vector<std::string> processedName;

    for (std::string& word : splitName){
        std::string lower = StrUtils::toLower(word);
        if (commonWords.find(lower) == commonWords.end()){
            processedName.push_back(removePlural(lower));
        }
    }

    std::sort(processedName.begin(), processedName.end());
    for (std::string& word : processedName){
        this->processedKey += word + "-";
    }

    this->ID = Crypto::hashToInt(this->processedKey);
}

void Entry::setCommitID(unsigned int commitID){
    this->commitID = commitID;
}

std::string Entry::constructEntryKey(unsigned int ID, unsigned int commitID){
    return Crypto::toHex(ID) + "-" + std::to_string(commitID);
}

std::string Entry::getKey(EntryType entryType){
    if (entryType == EntryType::EntryIDEntry){
        EntryType type = branchID == 0 ? EntryType::OfflineEntryIDEntry : EntryType::EntryIDEntry;
        return constructKey(type, constructEntryKey(this->ID, commitID));
    }

    return constructKey(entryType, Crypto::toHex(this->ID));
}

std::string Entry::getKey(){
    return constructEntryKey(this->ID, commitID);
}

std::string Entry::getProcessedKey(){
    return this->processedKey;
}
unsigned int Entry::getID(){
    return this->ID;
}
unsigned int Entry::getCommitID(){
    return this->commitID;
}

std::string Entry::getIDString(){
    return Crypto::toHex(this->ID);
} 

std::string Entry::getDate(){
    return this->date;
}
std::string Entry::getName(){
    return this->name;
}
std::string Entry::getEntry(){
    return this->entry;
}
std::string Entry::getData(){
    return this->data;
}
unsigned long long Entry::getBranchID(){
    return this->branchID;
}