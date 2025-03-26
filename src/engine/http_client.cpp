#include <engine/http_client.h>
#include <json.hpp>

using json = nlohmann::json;

HttpClient::HttpClient(std::string url, std::string dataPath)
    : url(url), dataPath(dataPath), client(url.c_str())
{}

HttpClient::~HttpClient()
{}

std::string HttpClient::read(std::string key){
    bool dummy;
    return read(dummy, key);
}

bool HttpClient::ping(std::string key){
    std::string value = "ping";
    bool success = write(key, value);
    if(!success)
        return false;

    std::string readValue = read(key);
    return readValue == value;
}

std::string HttpClient::read(bool& success, std::string key){
    bool dummy;
    return read(dummy, success, key);
}

std::string HttpClient::read(bool& received, bool& success, std::string key){
    received = false;
    success = false;

    httplib::Result res = client.Get((dataPath + "?key=" + key).c_str());
    if (!res) {
        return "";
    }
    received = true;

    if (res->status != 200) {
        return "";
    }
    success = true;

    try {
        json parsed = json::parse(res->body);
        return parsed["data"];
    } catch (...) {
        return "";
    }
}

bool HttpClient::write(std::string key, std::string value){
    // You no longer need to escape newlines manually — JSON handles this
    json payload;
    payload["key"] = key;
    payload["data"] = value;

    httplib::Result res = client.Post(dataPath.c_str(), payload.dump(), "application/json");

    if (!res)
        return false;
    return res->status == 200;
}