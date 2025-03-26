#include "user_tree.h"

#include <engine/input.h>
#include <engine/str_utils.h>

#include <engine/logger.h>

// UserTree* UserTree::instance = nullptr;

unsigned long long generateRandomID(){
    static std::mt19937 rng(std::random_device{}()); // Random number generator
    static std::uniform_int_distribution<unsigned long long> dist(0, std::numeric_limits<unsigned long long>::max());
    return dist(rng);
}

UserTree::UserTree(std::string url, std::string dataPath, unsigned long long steamID, Texture branchDiffuse, Texture branchNormal)
    : treeManager(nullptr), url(url), dataPath(dataPath), client(url, dataPath), branchDiffuse(branchDiffuse), branchNormal(branchNormal)
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
        int MAX_ITERATIONS = 100;
        for(int i = 0; i < MAX_ITERATIONS; i++){
            newID = generateRandomID();

            bool received;
            bool success;
            client.read(received, success, constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(newID)));

            if(!received){
                break;
            }

            if(!success){ // if key does not exist
                write(key, Crypto::toHex(newID));

                // migrate and create empty keys
                std::string entryData = SaveFile::read(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(id)));
                write(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(newID)), entryData);

                std::string branchData = SaveFile::read(constructKey(EntryType::BranchIDBranchIDs, Crypto::toHex(id)));
                write(constructKey(EntryType::BranchIDBranchIDs, Crypto::toHex(newID)), branchData);

                id = newID;

                //* individual entry data should be always checked to be migrated always
                break;
            }
        }
    }

    // migrate individual entries to new id (if applicable)
    if (id != 0){ //TODO make this better
        std::string entryData = SaveFile::read(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(id)));
        if (entryData != ""){
            std::vector<std::string> entryIDs = StrUtils::split(entryData, ',');
            bool migrated = false;
            for(int i = 0; i < entryIDs.size(); i++){
                Logger::log("Checking entry: " + entryIDs[i]);
                std::string key = constructKey(EntryType::EntryIDEntry, entryIDs[i]);
                std::string value = SaveFile::read(key);
                Entry entry = Entry(entryIDs[i], value);
                if (entry.getBranchID() == 0){ //* if offline entry
                    entry = Entry(id, entry.getDate(), entry.getName(), entry.getEntry());
                    if(writeEntryToServer(entry)){
                        migrated = true;
                    } 
                }

                // if (value != ""){ // ! this checks for if the server entry has been tampered with
                //     std::string clientValue = client.read(key);
                //     if (clientValue != value){
                //         Entry entry = Entry(entryIDs[i], value);
                //         entry = Entry(id, entry.getDate(), entry.getName(), entry.getEntry());

                //         if(writeEntryToServer(entry)){
                //             entryIDs[i] = entry.getKey();
                //             migrated = true;
                //         }
                //     }
                // }
            }
            if(migrated){
                std::string newEntryData = StrUtils::join(entryIDs, ',');
                write(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(id)), newEntryData);
            }
        }
    }

    treeManager = new TreeManager(id);

    loadBranch(0, SerializedBranch(id, 0, 0, 0, 0));
}

UserTree::~UserTree(){
    delete treeManager;
}

// void UserTree::loadBranch(unsigned long long branchID, unsigned long long parentBranchID, ){
void UserTree::loadBranch(unsigned long long parentID, SerializedBranch serializedBranch){
    //TODO add edge case fine combing.

    unsigned long long branchID = serializedBranch.branchID;
    TreeBranch* branch = treeManager->addBranch(branchID, parentID, Entry::constructEntryKey(serializedBranch.originKey, serializedBranch.idParent));
    branch->pushBackTexture(branchDiffuse);
    branch->pushBackTexture(branchNormal);
    branch->addShader(FRAME_BUFFER, "tree");
    branch->addShader(SHADOW_BUFFER, "treeShadowMap");
    branch->getLeafManager()->addShader(FRAME_BUFFER, "leaf");
    branch->getLeafManager()->addShader(SHADOW_BUFFER, "leafShadowMap");

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
            Logger::log("Loading Entry: " + entryID);
            if (value == ""){
                Logger::log("ERROR: Failed to load entry: " + key);
                continue;
            }

            Entry entry = Entry(entryID, value);
            branch->addNode(entry);
        }
        branch->recalculateVertices();
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

bool UserTree::write(std::string key, std::string value){
    int val = client.write(key, value); 
    if(!val){
        Logger::log("Write failed: " + std::to_string(val) + " " + key + " " + value);
    }

    SaveFile::write(key, value);
    return val;
}

void UserTree::writeAsync(std::string key, std::string value){
    fireAndForget([this, key, value] {
        int val = client.write(key, value);
        if(!val){
            Logger::log("Write failed: " + std::to_string(val) + " " + key + " " + value);
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

bool UserTree::writeEntryToServer(Entry& entry){
    bool received;
    bool success;
    std::string value = client.read(received, success, entry.getKey(EntryType::EntryIDCount));
    if (!received){
        return false;
    }

    int count = 0;
    if (value != ""){
        count = std::stoi(value);
    }
    entry.setCommitID(count);

    if(!write(entry.getKey(EntryType::EntryIDCount), std::to_string(count + 1))){
        return false;
    }

    if(!write(entry.getKey(EntryType::EntryIDEntry), entry.getData())){
        return false;
    }

    return true;
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

    if (!writeEntryToServer(entry)){
        Entry offlineEntry = Entry(0, date, name, mainEntry);
        int MAX_COUNT = 10000;
        int count = 0;
        std::string entryID = "";
        bool found = false;
        for (int i = 0; i < MAX_COUNT; i++){
            offlineEntry.setCommitID(i);
            if (!SaveFile::exists(offlineEntry.getKey(EntryType::EntryIDEntry))){
                found = true;
                break;
            }
            count++;
        }

        if (!found){
            return "!Use a different name";
        }

        SaveFile::write(offlineEntry.getKey(EntryType::EntryIDEntry), offlineEntry.getData());
    }

    TreeBranch* rootBranch = treeManager->rootBranch();
    rootBranch->addNode(entry);
    rootBranch->recalculateVertices();
    write(constructKey(EntryType::BranchIDEntryIDs, rootBranch->getIDString()), rootBranch->serializeNodes());
    return "";
}
