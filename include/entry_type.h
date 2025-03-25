#pragma once

#include <string>

enum class EntryType{
    UserIDBranchID, // UserID -> BranchID

    // BranchID -> BranchData(Root Branch Data, Attached Branch Data)
    BranchIDEntryIDs, // BranchID -> EntryIDs
    BranchIDBranchIDs, // BranchID -> BranchIDs

    EntryIDCount, // EntryID -> #

    EntryIDEntry, // EntryID-# -> Entry
    OfflineEntryIDEntry, // OfflineEntryID-# -> Entry

    Unknown,
};

inline std::string EntryTypeToString(EntryType entryType){
    switch (entryType){
        case EntryType::UserIDBranchID:
            return "UB";
        case EntryType::BranchIDEntryIDs:
            return "BE";
        case EntryType::BranchIDBranchIDs:
            return "BB";
        case EntryType::EntryIDCount:
            return "EC";
        case EntryType::EntryIDEntry:
            return "EE";
        case EntryType::OfflineEntryIDEntry:
            return "OE";
        default:
            return "XX";
    }
}

inline EntryType StringToEntryType(std::string entryType){
    if (entryType == "UB"){
        return EntryType::UserIDBranchID;
    }else if (entryType == "BE"){
        return EntryType::BranchIDEntryIDs;
    }else if (entryType == "BB"){
        return EntryType::BranchIDBranchIDs;
    }else if (entryType == "EC"){
        return EntryType::EntryIDCount;
    }else if (entryType == "EE"){
        return EntryType::EntryIDEntry;
    }else if (entryType == "OE"){
        return EntryType::OfflineEntryIDEntry;
    }else{
        return EntryType::Unknown;
    }
}

inline std::string constructKey(EntryType type, std::string key){
    return EntryTypeToString(type) + "-" + key;
}