#include <engine/http_client.h>

HttpClient::HttpClient(std::string url, std::string dataPath)
    : url(url), dataPath(dataPath), client(url.c_str())
{}

HttpClient::~HttpClient()
{}

std::string HttpClient::read(std::string key){
    auto res = client.Get((dataPath + "?key=" + key).c_str());
    if(!res)
        return "";
    if(res->status != 200){
        return "";
    }

    std::string body = res->body;
    std::string data = body.substr(body.find(":") + 3);
    data = data.substr(0, data.size() - 2);
    return data;
}

bool HttpClient::write(std::string key, std::string value){
    value = "{\"key\": \"" + key + "\", \"data\": \"" + value + "\"}";
    auto res = client.Post((dataPath).c_str(), value, "application/json");
    if(!res)
        return 0;
    return res->status == 200;
}

