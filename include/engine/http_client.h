#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <httplib.h>
#include <string>

class HttpClient {
public:
    HttpClient(std::string url, std::string dataPath);
    ~HttpClient();

    bool ping(std::string key);
    std::string read(std::string key);
    std::string read(bool& success, std::string key);
    std::string read(bool& received, bool& success, std::string key);

    bool write(std::string key, std::string value);
private:
    httplib::Client client;
    std::string url;
    std::string dataPath;
};

#endif // HTTP_CLIENT_H