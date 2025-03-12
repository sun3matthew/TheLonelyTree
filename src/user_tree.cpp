
#include "user_tree.h"
#include <engine/save_file.h>
#include <engine/crypto.h>

UserTree::UserTree(std::string url, std::string dataPath)
    : treeManager(nullptr), url(url), dataPath(dataPath), client(url, dataPath)
{
    counter = 0;
}

UserTree::~UserTree(){
}

void UserTree::updateTreeManager(unsigned long long userID, TreeManager* treeManager){
    this->treeManager = treeManager;
    int result = write(constructKey(EntryType::UserIDBranchID, Crypto::toHex(userID)), treeManager->rootBranch()->getIDString());
}

std::string UserTree::constructKey(EntryType type, std::string key){
    return EntryTypeToString(type) + "-" + key;
}

std::string UserTree::constructKey(EntryType type, unsigned int secondaryID, std::string key){
    return EntryTypeToString(type) + "-" + key + "-" + std::to_string(secondaryID);
}

bool UserTree::write(std::string key, std::string value){
    int val = client.write(key, value);
    if(!val){
        std::cout << "Write failed: " << val << " " << key << " " << value << std::endl;
    }

    SaveFile::Save(key, value);
    return val;
}

std::string UserTree::read(std::string key){
    std::string value = client.read(key);
    if (value == ""){
        value = SaveFile::Load(key);
    }
    return value;
}

void UserTree::update(){
    counter++;
    // std::cout << "Counter: " << counter << std::endl;
    // if (counter % 100 == 0)
    //     client.read(constructKey(EntryType::UserIDBranchID, "0"));
}

void UserTree::addEntry(Entry entry){
    // std::string url = "https://7sqvdwegyf.execute-api.us-west-2.amazonaws.com";
    // std::string dataPath = "/default/the-lonely-tree";

    //TODO change such that server has more checks. Add "append" functionality
    // TODO add all checks for if malicious data gets sent on the server

    std::string readCount = read(constructKey(EntryType::EntryPCount, entry.getProcessedKeyHash()));
    unsigned int count = 0;
    if (readCount != ""){
        count = std::stoi(readCount);
    }

    treeManager->rootBranch()->addNode(entry);

    int result = 0;
    result = write(constructKey(EntryType::EntryIDEntry, entry.getHashString()), entry.getData());
    result = write(constructKey(EntryType::EntryPCount, entry.getProcessedKeyHash()), std::to_string(count + 1));
    result = write(constructKey(EntryType::EntryPEntryID, count, entry.getProcessedKeyHash()), entry.getHashString());
    result = write(constructKey(EntryType::BranchIDEntryIDs, treeManager->rootBranch()->getIDString()), treeManager->rootBranch()->serializeNodes());
}