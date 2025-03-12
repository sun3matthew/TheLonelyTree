#include <engine/crypto.h>

#include "entry.h"


// Entry(std::string date, std::string name, std::string data);
// ~Entry();

// std::string getDate();
// std::string getName();
// std::string getData();

// std::string getProcessedKey();
// private:
// std::string date;
// std::string name;
// std::string data;

// std::string processedKey;

// unsigned long long hash;
// std::string hashString;

const std::unordered_set<std::string> Entry::commonWords = {
    "the", "is", "in", "and", "of", "to", "a", "with", "for", "on", "at", "by", "from", "an", "as",
    "be", "this", "that", "it", "or", "but", "not", "are", "were", "will", "would", "shall", "can", 
    "all", "any", "each", "every", "some", "so", "up", "down", "into", "out", "about", "over", 
    "under", "again", "further", "then", "once", "here", "there", "how", "why", "when", "where", 
    "who", "whom", "which", "whose", "you", "we", "they", "i", "me", "us", "he", "him", "she", 
    "her", "it", "they", "them", "my", "your", "his", "her", "its", "our", "their", "ours", "yours"
};

std::vector<std::string> split(std::string& str){
    std::vector<std::string> splitName;
    std::string temp = "";
    for (char c : str){
        if (c == ' '){
            splitName.push_back(temp);
            temp = "";
        } else {
            if (isalnum(c)){
                temp += toupper(c);
            }
        }
    }
    splitName.push_back(temp);
    return splitName;
}

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

Entry::Entry(std::string date, std::string name, std::string entry){
    std::replace(date.begin(), date.end(), '\n', ' ');
    std::replace(name.begin(), name.end(), '\n', ' ');

    this->date = date;
    this->name = name;
    this->entry = entry;

    this->data = date + "\n" + name + "\n" + entry;

    this->hash = Crypto::fnv1a_hash(data);
    this->hashString = Crypto::toHex(this->hash);

    // * Simplifies key to it's base parts, merges "similar entries"
    // split name by spaces, remove common words, remove special characters
    std::vector<std::string> splitName = split(name);
    std::vector<std::string> processedName;

    for (std::string& word : splitName){
        if (commonWords.find(word) == commonWords.end()){
            processedName.push_back(removePlural(word));
        }
    }

    std::sort(processedName.begin(), processedName.end());
    for (std::string& word : processedName){
        this->processedKey += word + "-";
    }

    this->processedKeyHash = Crypto::toHex(Crypto::hashToInt(this->processedKey));
}

Entry::~Entry(){
}

std::string Entry::getProcessedKey(){
    return this->processedKey;
}
std::string Entry::getProcessedKeyHash(){
    return this->processedKeyHash;
}

unsigned long long Entry::getHash(){
    return this->hash;
}
std::string Entry::getHashString(){
    return this->hashString;
}


std::string Entry::getDate(){
    return this->date;
}
std::string Entry::getName(){
    return this->name;
}
std::string Entry::getData(){
    return this->data;
}