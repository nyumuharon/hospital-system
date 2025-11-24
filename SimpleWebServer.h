#ifndef SIMPLEWEBSERVER_H
#define SIMPLEWEBSERVER_H

#include <string>
#include <vector>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "HospitalController.h"

#pragma comment(lib, "Ws2_32.lib")

class SimpleWebServer {
private:
    SOCKET serverSocket;
    HospitalController* controller;
    bool isRunning;
    int port;

    void handleClient(SOCKET clientSocket);
    std::string getContentType(const std::string& path);
    std::string readFile(const std::string& path);
    
    // API Handlers
    std::string handleApiRequest(const std::string& method, const std::string& path, const std::string& body);
    std::string jsonDrugs();
    std::string jsonReport();
    std::string jsonDashboard();
    std::string jsonLogin(const std::string& body);

public:
    SimpleWebServer(HospitalController* ctrl, int p = 8080);
    ~SimpleWebServer();
    void start();
    void stop();
};

#endif
