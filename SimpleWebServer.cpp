#include "SimpleWebServer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

SimpleWebServer::SimpleWebServer(HospitalController* ctrl, int p) : controller(ctrl), port(p), isRunning(false), serverSocket(INVALID_SOCKET) {}

SimpleWebServer::~SimpleWebServer() {
    stop();
}

void SimpleWebServer::stop() {
    isRunning = false;
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
    WSACleanup();
}

void SimpleWebServer::start() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed.\n";
        return;
    }

    struct addrinfo *result = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    string portStr = to_string(port);
    if (getaddrinfo(NULL, portStr.c_str(), &hints, &result) != 0) {
        cerr << "getaddrinfo failed.\n";
        WSACleanup();
        return;
    }

    serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error at socket(): " << WSAGetLastError() << "\n";
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    if (bind(serverSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        cerr << "bind failed with error: " << WSAGetLastError() << "\n";
        freeaddrinfo(result);
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed with error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    isRunning = true;
    cout << "Web Server started on http://localhost:" << port << "\n";

    while (isRunning) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            if (isRunning) cerr << "accept failed: " << WSAGetLastError() << "\n";
            continue;
        }
        
        // Handle in a new thread (detached for simplicity in this demo)
        thread(&SimpleWebServer::handleClient, this, clientSocket).detach();
    }
}

void SimpleWebServer::handleClient(SOCKET clientSocket) {
    try {
        char buffer[4096];
        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        if (bytesReceived > 0) {
            string request(buffer, bytesReceived);
            istringstream iss(request);
            string method, path, protocol;
            iss >> method >> path >> protocol;

            cout << "Request: " << method << " " << path << "\n";
            
            string response;
            string contentType = "text/html";
            
            // CORS Preflight
            if (method == "OPTIONS") {
                response = "";
                contentType = "text/plain";
            } else {
                // Simple routing
                if (path.find("/api/") == 0) {
                    // Extract body if POST
                    string body = "";
                    size_t bodyPos = request.find("\r\n\r\n");
                    if (bodyPos != string::npos) {
                        body = request.substr(bodyPos + 4);
                    }
                    response = handleApiRequest(method, path, body);
                    contentType = "application/json";
                } else {
                    if (path == "/") path = "/index.html";
                    // Remove leading slash for file path
                    string filePath = "www" + path; 
                    response = readFile(filePath);
                    contentType = getContentType(filePath);
                    if (response.empty()) {
                        response = "<h1>404 Not Found</h1>";
                        contentType = "text/html";
                    }
                }
            }

            string header = "HTTP/1.1 200 OK\r\n"
                                 "Content-Type: " + contentType + "\r\n"
                                 "Content-Length: " + to_string(response.size()) + "\r\n"
                                 "Cache-Control: no-cache, no-store, must-revalidate\r\n"
                                 "Pragma: no-cache\r\n"
                                 "Expires: 0\r\n"
                                 "Access-Control-Allow-Origin: *\r\n"
                                 "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                                 "Access-Control-Allow-Headers: Content-Type\r\n"
                                 "\r\n";
            
            send(clientSocket, header.c_str(), header.size(), 0);
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    } catch (const exception& e) {
        cerr << "Error in handleClient: " << e.what() << "\n";
    } catch (...) {
        cerr << "Unknown error in handleClient\n";
    }
    closesocket(clientSocket);
}

string SimpleWebServer::getContentType(const string& path) {
    if (path.find(".html") != string::npos) return "text/html";
    if (path.find(".css") != string::npos) return "text/css";
    if (path.find(".js") != string::npos) return "application/javascript";
    return "text/plain";
}

string SimpleWebServer::readFile(const string& path) {
    ifstream file(path, ios::binary);
    if (!file) return "";
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// --- API Implementation ---

string SimpleWebServer::handleApiRequest(const string& method, const string& path, const string& body) {
    if (path == "/api/drugs" && method == "GET") return jsonDrugs();
    if (path == "/api/report" && method == "GET") return jsonReport();
    if (path == "/api/dashboard" && method == "GET") return jsonDashboard();
    if (path == "/api/login" && method == "POST") return jsonLogin(body);
    return "{\"error\": \"Endpoint not found\"}";
}

string SimpleWebServer::jsonDashboard() {
    float revenue = controller->getDailyRevenue();
    
    int lowStockCount = 0;
    for (const auto& d : controller->getDrugs()) {
        if (d.isLowStock()) lowStockCount++;
    }

    int pendingRx = controller->getPendingPrescriptions().size();

    auto& transactions = controller->getTransactions();
    string recentTx = "[";
    int count = 0;
    // Get last 5 transactions
    for (auto it = transactions.rbegin(); it != transactions.rend() && count < 5; ++it, ++count) {
        recentTx += "{";
        recentTx += "\"id\": " + to_string(it->id) + ",";
        recentTx += "\"amount\": " + to_string(it->amount) + ",";
        recentTx += "\"date\": \"" + it->date + "\",";
        recentTx += "\"method\": \"" + it->paymentMethod + "\"";
        recentTx += "}";
        if (count < 4 && count < transactions.size() - 1) recentTx += ",";
    }
    recentTx += "]";

    string json = "{";
    json += "\"revenue\": " + to_string(revenue) + ",";
    json += "\"lowStock\": " + to_string(lowStockCount) + ",";
    json += "\"pendingRx\": " + to_string(pendingRx) + ",";
    json += "\"recentTransactions\": " + recentTx;
    json += "}";
    return json;
}

string SimpleWebServer::jsonDrugs() {
    auto& drugs = controller->getDrugs();
    string json = "[";
    for (size_t i = 0; i < drugs.size(); ++i) {
        json += "{";
        json += "\"name\": \"" + drugs[i].name + "\",";
        json += "\"quantity\": " + to_string(drugs[i].quantity) + ",";
        json += "\"price\": " + to_string(drugs[i].price) + ",";
        json += "\"expiry\": \"" + drugs[i].expiryDate + "\"";
        json += "}";
        if (i < drugs.size() - 1) json += ",";
    }
    json += "]";
    return json;
}

string SimpleWebServer::jsonReport() {
    string json = "{";
    json += "\"revenue\": " + to_string(controller->getDailyRevenue()) + ",";
    json += "\"total_users\": " + to_string(DataManager::loadUsers().size()); // Direct load for simplicity or add getter to controller
    json += "}";
    return json;
}

string SimpleWebServer::jsonLogin(const string& body) {
    // Very simple parsing for "username=foo&password=bar" or JSON
    // Assuming JSON for simplicity in this "AI" context, but let's do simple string find
    // Actually, let's assume the frontend sends raw JSON: {"username": "u", "password": "p"}
    
    string u, p;
    size_t uPos = body.find("\"username\":");
    if (uPos != string::npos) {
        size_t start = body.find("\"", uPos + 11) + 1;
        size_t end = body.find("\"", start);
        u = body.substr(start, end - start);
    }
    
    size_t pPos = body.find("\"password\":");
    if (pPos != string::npos) {
        size_t start = body.find("\"", pPos + 11) + 1;
        size_t end = body.find("\"", start);
        p = body.substr(start, end - start);
    }

    if (controller->login(u, p)) {
        User* user = controller->getCurrentUser();
        return "{\"success\": true, \"role\": \"" + user->getRoleString() + "\"}";
    }
    return "{\"success\": false}";
}
