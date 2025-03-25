#ifndef USER_TREE_H
#define USER_TREE_H

#include <engine/component.h>
#include <engine/http_client.h>
#include <engine/save_file.h>
#include <engine/crypto.h>
#include <future>

#include "entry_type.h"
#include "entry.h"
#include "tree_manager.h"

class UserTree : public Component{
    public:
        UserTree(std::string url, std::string dataPath, unsigned long long steamID); 
        ~UserTree() override;

        TreeManager* getTreeManager();

        void update() override;

        std::string addEntry(std::string date, std::string name, std::string entry);

        void write(std::string key, std::string value);
        void writeAsync(std::string key, std::string value);

        std::string read(std::string key);

        void readAsync(std::string key);
        std::string getAsyncResult();
        bool isAsyncDone();

    private:
        void loadBranch(unsigned long long parentID, SerializedBranch serializedBranch);

        EntryType readEntryType;
        Entry* currentEntry;
        std::future<std::string> future;

        TreeManager* treeManager;
        std::string url;
        std::string dataPath;

        HttpClient client;

        float counter;
};

template<typename F>
void fireAndForget(F&& f) {
    std::thread(std::forward<F>(f)).detach();
}


#endif // USER_TREE_H