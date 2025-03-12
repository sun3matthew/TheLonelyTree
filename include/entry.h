#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <unordered_set>

class Entry{
    public:
        Entry(std::string date, std::string name, std::string entry);
        ~Entry();

        std::string getDate();
        std::string getName();
        std::string getData();

        std::string getProcessedKey();
        std::string getProcessedKeyHash();
        std::string getHashString();
        unsigned long long getHash();
    private:
        std::string date;
        std::string name;
        std::string entry;
        std::string data;

        std::string processedKey;
        std::string processedKeyHash;

        unsigned long long hash;
        std::string hashString;

        static const std::unordered_set<std::string> commonWords;
};

#endif // ENTRY_H