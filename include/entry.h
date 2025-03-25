#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <unordered_set>

#include "entry_type.h"

class Entry{
    public:
        static std::string constructEntryKey(unsigned int ID, unsigned int commitID);

        Entry(unsigned long long branchID, std::string date, std::string name, std::string entry);
        Entry(std::string key, std::string data);
        ~Entry();

        void setCommitID(unsigned int commitID);

        std::string getDate();
        std::string getName();
        std::string getEntry();

        std::string getData();

        std::string getProcessedKey();

        unsigned int getID();
        bool isCommited();
        unsigned int getCommitID();
        unsigned long long getBranchID();

        std::string getKey(EntryType);
        std::string getKey();

        std::string getIDString();

    private:
        void recalculateKey();

        std::string date;
        std::string name;
        std::string entry;
        std::string data;

        std::string processedKey;

        unsigned long long branchID;

        unsigned int ID;
        unsigned int commitID;

        static const std::unordered_set<std::string> commonWords;
};

#endif // ENTRY_H