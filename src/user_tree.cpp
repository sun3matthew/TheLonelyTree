
#include "user_tree.h"
#include <engine/save_file.h>
#include <engine/crypto.h>
#include <iostream>
#include <engine/input.h>
#include <future>

UserTree::UserTree(std::string url, std::string dataPath)
    : treeManager(nullptr), url(url), dataPath(dataPath), client(url, dataPath)
{
    counter = 0;
}

UserTree::~UserTree(){
}

void UserTree::updateTreeManager(unsigned long long userID, TreeManager* treeManager){
    this->treeManager = treeManager;
    write(constructKey(EntryType::UserIDBranchID, Crypto::toHex(userID)), treeManager->rootBranch()->getIDString());
}

std::string UserTree::constructKey(EntryType type, std::string key){
    return EntryTypeToString(type) + "-" + key;
}

std::string UserTree::constructKey(EntryType type, unsigned int secondaryID, std::string key){
    return EntryTypeToString(type) + "-" + key + "-" + std::to_string(secondaryID);
}

void UserTree::write(std::string key, std::string value){
    int val = client.write(key, value);
    if(!val){
        std::cout << "Write failed: " << val << " " << key << " " << value << std::endl;
    }

    SaveFile::Save(key, value);
}

void UserTree::writeAsync(std::string key, std::string value){
    fireAndForget([this, key, value] {
        int val = client.write(key, value);
        if(!val){
            std::cout << "Write failed: " << val << " " << key << " " << value << std::endl;
        }

        SaveFile::Save(key, value);
    });
}

std::string UserTree::read(std::string key){
    std::string value = client.read(key);
    if (value == ""){
        value = SaveFile::Load(key);
    }
    return value;
}

void UserTree::readAsync(std::string key){
    if (future.valid() && future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
        return;
    }

    future = std::async(std::launch::async, [this, key] {
        std::string value = client.read(key);
        if (value == ""){
            value = SaveFile::Load(key);
        }
        return value;
    });
}

bool UserTree::isAsyncDone(){
    return future.valid() && future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
}

std::string UserTree::getAsyncResult(){
    if (isAsyncDone()) {
        return future.get();
    }
    return "";
}


void UserTree::update(){
    if (Input::getKeyDown(KeyCode::KEY_F9)){
        // writeAsync("test", "test value test");
        readAsync("test");
    }

    std::cout << "Result: " << getAsyncResult() << std::endl;

    counter++;

    int numNodes = treeManager->rootBranch()->getNumNodes();


    // std::cout << "Counter: " << counter << std::endl;
    // if (counter % 100 == 0)
    //     client.read(constructKey(EntryType::UserIDBranchID, "0"));
}

void UserTree::addEntry(Entry entry){
    //TODO change such that server has more checks. Add "append" functionality
    // TODO add all checks for if malicious data gets sent on the server

    std::string readCount = read(constructKey(EntryType::EntryPCount, entry.getProcessedKeyHash()));
    unsigned int count = 0;
    if (readCount != ""){
        count = std::stoi(readCount);
    }

    treeManager->rootBranch()->addNode(entry);

    write(constructKey(EntryType::EntryIDEntry, entry.getHashString()), entry.getData());
    write(constructKey(EntryType::EntryPCount, entry.getProcessedKeyHash()), std::to_string(count + 1));
    write(constructKey(EntryType::EntryPEntryID, count, entry.getProcessedKeyHash()), entry.getHashString());
    write(constructKey(EntryType::BranchIDEntryIDs, treeManager->rootBranch()->getIDString()), treeManager->rootBranch()->serializeNodes());
}