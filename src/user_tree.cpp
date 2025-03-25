#include "user_tree.h"

#include <iostream>

#include <engine/input.h>
#include <engine/str_utils.h>

// UserTree* UserTree::instance = nullptr;

unsigned long long generateRandomID(){
    static std::mt19937 rng(std::random_device{}()); // Random number generator
    static std::uniform_int_distribution<unsigned long long> dist(0, std::numeric_limits<unsigned long long>::max());
    return dist(rng);
}

UserTree::UserTree(std::string url, std::string dataPath, unsigned long long steamID)
    : treeManager(nullptr), url(url), dataPath(dataPath), client(url, dataPath)
{
    // UserTree::instance = this;

    counter = 0;
    readEntryType = EntryType::Unknown;

    // ! If it is uninitialized then see if you can generate a userID->branchID pair.
    // ! if you can't then use 0 as a temp
    // ! during generation, you have to make sure you move over the temp
    std::string key = constructKey(EntryType::UserIDBranchID, Crypto::toHex(steamID));
    std::string baseBranchID = read(key);
    unsigned long long id = 0;
    if (baseBranchID != ""){ // ? remove ?
        id = Crypto::hexToLong(baseBranchID);
    }

    if (id == 0){ // Attempt to create a new ID
        unsigned long long newID;
        int MAX_ITERATIONS = 1000;
        for(int i = 0; i < MAX_ITERATIONS; i++){
            newID = generateRandomID();
            if(client.write(key, Crypto::toHex(newID))){
                // migrate

                std::string entryData = SaveFile::read(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(id)));
                if (entryData != "")
                    write(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(newID)), entryData);

                std::string branchData = SaveFile::read(constructKey(EntryType::BranchIDBranchIDs, Crypto::toHex(id)));
                if (branchData != "")
                    write(constructKey(EntryType::BranchIDBranchIDs, Crypto::toHex(newID)), branchData);

                id = newID;

                //! individual entry data should be always checked to be migrated always
                break;
            }
        }
    }

    // migrate individual entries to new id (if applicable)
    if (id != 0){ //TODO make this better
        std::string entryData = SaveFile::read(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(id)));
        if (entryData != ""){
            std::vector<std::string> entryIDs = StrUtils::split(entryData, ',');
            for(int i = 0; i < entryIDs.size(); i++){
                std::string key = constructKey(EntryType::EntryIDEntry, entryIDs[i]);
                std::string value = SaveFile::read(key);
                if (value != ""){
                    Entry entry = Entry(entryIDs[i], value);
                    if(entry.getBranchID() == 0){
                        entry = Entry(id, entry.getDate(), entry.getName(), entry.getEntry());
                        write(entry.getKey(EntryType::EntryIDEntry), entry.getData());
                        entryIDs[i] = entry.getKey();
                    }
                }
            }
            std::string newEntryData = StrUtils::join(entryIDs, ',');
            write(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(id)), newEntryData);
        }
    }

    treeManager = new TreeManager(id);

    loadBranch(0, SerializedBranch(id, 0, 0, 0, 0));
}

UserTree::~UserTree(){
}

// void UserTree::loadBranch(unsigned long long branchID, unsigned long long parentBranchID, ){
void UserTree::loadBranch(unsigned long long parentID, SerializedBranch serializedBranch){
    //TODO add edge case fine combing.

    unsigned long long branchID = serializedBranch.branchID;
    TreeBranch* branch = treeManager->addBranch(branchID, parentID, Entry::constructEntryKey(serializedBranch.originKey, serializedBranch.idParent));

    std::string entryData = "";
    std::string branchData = "";
    if (branchID == treeManager->rootBranchID){
        entryData = SaveFile::read(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(branchID)));
        branchData = SaveFile::read(constructKey(EntryType::BranchIDBranchIDs, Crypto::toHex(branchID)));
    }
    if (entryData == "" && branchData == ""){
        entryData = read(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(branchID)));
        branchData = read(constructKey(EntryType::BranchIDBranchIDs, Crypto::toHex(branchID)));
    }



    if (entryData != ""){
        std::vector<std::string> entryIDs = StrUtils::split(entryData, ',');
        for(std::string entryID : entryIDs){
            std::string key = constructKey(EntryType::EntryIDEntry, entryID);
            std::string value = SaveFile::read(key);
            if (value == ""){
                value = client.read(key); // do this so that it does not override cache
            }
            if (value == ""){
                std::cerr << "Failed to load entry: " << key << std::endl;
                continue;
            }

            Entry entry = Entry(entryID, value);
            branch->addNode(entry);
        }
    }

    if (branchData != ""){
        // ParentNode:NewID:{metadata},...,...
        std::vector<std::string> branchDataList = StrUtils::split(branchData, ',');
        for(std::string branchData : branchDataList){
            // TODO add tree energy attenuation
            SerializedBranch serializedBranch = SerializedBranch(branchData);
            loadBranch(branchID, serializedBranch);
        }
    }
}

TreeManager* UserTree::getTreeManager(){
    return treeManager;
}

void UserTree::write(std::string key, std::string value){
    int val = client.write(key, value); 
    if(!val){
        std::cout << "Write failed: " << val << " " << key << " " << value << std::endl;
    }

    SaveFile::write(key, value);
}

void UserTree::writeAsync(std::string key, std::string value){
    fireAndForget([this, key, value] {
        int val = client.write(key, value);
        if(!val){
            std::cout << "Write failed: " << val << " " << key << " " << value << std::endl;
        }

        SaveFile::write(key, value);
    });
}

std::string UserTree::read(std::string key){
    bool success;
    std::string value = client.read(success, key);
    if (!success){
        value = SaveFile::read(key);
    }else{
        SaveFile::write(key, value);
    }
    return value;
}

void UserTree::readAsync(std::string key){
    if (future.valid() && future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
        return;
    }

    future = std::async(std::launch::async, [this, key] {
        bool success;
        std::string value = client.read(success, key);
        if (!success){
            value = SaveFile::read(key);
        }else{
            SaveFile::write(key, value);
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
        readAsync("test");
    }


    // counter += GLFWWrapper::instance->getDeltaTime();
    if (counter > 10){
        if(readEntryType == EntryType::Unknown){
            int numNodes = treeManager->rootBranch()->getNumNodes();
            int currentNode = rand() % numNodes;

            currentEntry = &treeManager->rootBranch()->getNode(currentNode)->getEntry();
            std::cout << "Entry: " << currentEntry->getName() << std::endl;

            readEntryType = EntryType::EntryIDCount;
            readAsync(currentEntry->getKey(readEntryType));
        }else if(readEntryType == EntryType::EntryIDCount){
            if(isAsyncDone()){
                std::string readCount = getAsyncResult();
                std::cout << "Count: " << readCount << std::endl;
                unsigned int count = 0;
                if (readCount != ""){
                    count = std::stoi(readCount);
                }

                if (count <= 1){
                // if (count == 0){
                    counter = 0;
                    readEntryType = EntryType::Unknown;
                }else{
                    int randomEntry = rand() % count;

                    //TODO skip you own entries
                    if(currentEntry->getCommitID() == randomEntry){
                        counter = 0;
                        readEntryType = EntryType::Unknown;
                    }else{
                        readEntryType = EntryType::EntryIDEntry;
                        readAsync(currentEntry->getKey(EntryType::EntryIDEntry));
                    }
                }

            }
        }else if(readEntryType == EntryType::EntryIDEntry){
            if(isAsyncDone()){
                std::string entry = getAsyncResult();
                std::cout << "Entry: " << entry << std::endl;

                readEntryType = EntryType::Unknown;
                counter = 0;
            }
        }


        

        // for (int i = 0; i < numNodes; i++){
        //     Entry entry = treeManager->rootBranch()->getNode(i)->getEntry();
        //     std::string readCount = read(constructKey(EntryType::EntryPCount, entry.getProcessedKeyHash()));
        //     unsigned int count = 0;
        //     if (readCount != ""){
        //         count = std::stoi(readCount);
        //     }

        //     for (int j = 0; j < count; j++){
        //         std::string entryID = read(constructKey(EntryType::EntryPEntryID, j, entry.getProcessedKeyHash()));
        //         std::string entryData = read(constructKey(EntryType::EntryIDEntry, entryID));
        //         std::cout << entryData << std::endl;
        //     }
        // }
        
        // std::cout << "Counter: " << counter << std::endl;
        // client.read(constructKey(EntryType::UserIDBranchID, "0"));
    }

}

std::string UserTree::addEntry(std::string date, std::string name, std::string mainEntry){
    Entry entry = Entry(treeManager->rootBranch()->getID(), date, name, mainEntry);

    //TODO change such that server has more checks. Add "append" functionality
    // TODO add all checks for if malicious data gets sent on the server

    // TODO add check if entry name is already in the tree
    if (entry.getProcessedKey() == ""){
        return "!Use a more descriptive name";
    }

    TreeBranch* branch = treeManager->rootBranch();
    for (int i = 0; i < branch->getNumNodes(); i++){
        Entry nodeEntry = branch->getNode(i)->getEntry();
        // TODO add check if entry name is already in the tree
        // if (nodeEntry.getProcessedKey
        //     return false;
        // }
    }


    // std::string readCount = read(constructKey(EntryType::EntryPCount, entry.getProcessedKeyHash()));
    bool success;
    std::string value = client.read(success, entry.getKey(EntryType::EntryIDCount));
    int count = 0;
    if (success){
        count = 0;
        if (value != ""){
            count = std::stoi(value);
        }

        entry.setCommitID(count);

        write(entry.getKey(EntryType::EntryIDEntry), entry.getData());
        write(entry.getKey(EntryType::EntryIDCount), std::to_string(count + 1));
    }else{
        int MAX_COUNT = 10000;
        std::string entryID = "";
        bool found = false;
        for (int i = 0; i < MAX_COUNT; i++){
            entry.setCommitID(i);
            if (!SaveFile::exists(entry.getKey(EntryType::EntryIDEntry))){
                found = true;
                break;
            }
            count++;
        }

        if (!found){
            return "!Use a different name";
        }

        SaveFile::write(entry.getKey(EntryType::EntryIDEntry), entry.getData());
    }

    treeManager->rootBranch()->addNode(entry);
    TreeBranch* rootBranch = treeManager->rootBranch();
    write(constructKey(EntryType::BranchIDEntryIDs, rootBranch->getIDString()), rootBranch->serializeNodes());
    return "";
}
