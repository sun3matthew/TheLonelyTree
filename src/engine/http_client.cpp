#include <engine/http_client.h>

HttpClient::HttpClient(std::string url, std::string dataPath)
    : url(url), dataPath(dataPath), client(url.c_str())
{}

HttpClient::~HttpClient()
{}

std::string HttpClient::read(std::string key){
    bool dummy;
    return read(dummy, key);
}

std::string HttpClient::read(bool& success, std::string key){
    httplib::Result res = client.Get((dataPath + "?key=" + key).c_str());
    if(!res){
        success = false;
        return "";
    }
    if(res->status != 200){
        success = false;
        return "";
    }

    success = true;
    std::string body = res->body;
    std::string data = body.substr(body.find(":") + 3);
    data = data.substr(0, data.size() - 2);
    return data;
}

bool HttpClient::write(std::string key, std::string value){
    size_t pos = 0;
    while ((pos = value.find('\n', pos)) != std::string::npos) {
        value.replace(pos, 1, "\\n");
        pos += 2;
    }

    std::string payload = "{\"key\": \"" + key + "\", \"data\": \"" + value + "\"}";
    auto res = client.Post(dataPath.c_str(), payload, "application/json");

    if (!res)
        return false;
    return res->status == 200;
}

