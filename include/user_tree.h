#ifndef USER_TREE_H
#define USER_TREE_H

#include <engine/component.h>
#include <engine/http_client.h>

#include "entry.h"
#include "tree_manager.h"

enum class EntryType{
    UserIDBranchID,

    BranchIDEntryIDs,
    BranchIDBranchIDs,

    EntryIDEntry,

    EntryPCount,
    EntryPEntryID,
};

inline std::string EntryTypeToString(EntryType entryType){
    switch (entryType){
        case EntryType::BranchIDEntryIDs:
            return "BE";
        case EntryType::BranchIDBranchIDs:
            return "BB";
        case EntryType::EntryIDEntry:
            return "EE";
        case EntryType::EntryPCount:
            return "PC";
        case EntryType::EntryPEntryID:
            return "PE";
        case EntryType::UserIDBranchID:
            return "UB";
        default:
            return "XX";
    }
}

class UserTree : public Component{
    public:
        UserTree(std::string url, std::string dataPath);
        ~UserTree() override;

        void updateTreeManager(unsigned long long userID, TreeManager* treeManager);

        void update() override;

        void addEntry(Entry entry);

        std::string constructKey(EntryType type, std::string key);
        std::string constructKey(EntryType type, unsigned int secondaryID, std::string key);

        bool write(std::string key, std::string value);
        std::string read(std::string key);

    private:
        TreeManager* treeManager;
        std::string url;
        std::string dataPath;

        HttpClient client;

        int counter;
};

#endif // USER_TREE_H