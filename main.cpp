#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

// 简单的学生类
class Student {
public:
    std::string id;
    std::string name;
    double score1, score2, score3;
    
    Student() : score1(0), score2(0), score3(0) {}
    
    Student(std::string i, std::string n, double s1, double s2, double s3) 
        : id(i), name(n), score1(s1), score2(s2), score3(s3) {}
    
    // 计算总分
    double getTotal() {
        return score1 + score2 + score3;
    }
    
    // 计算平均分
    double getAvg() {
        return getTotal() / 3.0;
    }
    
    // 转为JSON
    std::string toJson() {
        std::stringstream ss;
        ss << "{";
        ss << "\"id\":\"" << id << "\",";
        ss << "\"name\":\"" << name << "\",";
        ss << "\"scores\":[" << score1 << "," << score2 << "," << score3 << "],";
        ss << "\"total\":" << getTotal() << ",";
        ss << "\"average\":" << getAvg();
        ss << "}";
        return ss.str();
    }
};

// 全局学生列表
std::vector<Student> students;

// 从文件加载数据
void loadData() {
    students.clear();
    std::ifstream file("data/students.txt");
    if (!file.is_open()) {
        // 如果文件不存在，就创建一个空文件
        std::ofstream createFile("data/students.txt");
        return;
    }
    
    std::string id, name;
    double s1, s2, s3;
    while (file >> id >> name >> s1 >> s2 >> s3) {
        students.push_back(Student(id, name, s1, s2, s3));
    }
    file.close();
}

// 保存数据到文件
void saveData() {
    std::ofstream file("data/students.txt");
    for (auto& s : students) {
        file << s.id << " " << s.name << " " 
             << s.score1 << " " << s.score2 << " " << s.score3 << std::endl;
    }
    file.close();
}

// 处理HTTP请求
std::string handleRequest(const std::string& request) {
    // 如果是获取所有学生
    if (request.find("GET /api/students") != std::string::npos) {
        loadData();
        std::string json = "[";
        for (size_t i = 0; i < students.size(); i++) {
            if (i > 0) json += ",";
            json += students[i].toJson();
        }
        json += "]";
        
        return "HTTP/1.1 200 OK\r\n"
               "Access-Control-Allow-Origin: *\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(json.length()) + "\r\n"
               "\r\n" + json;
    }
    
    // 如果是添加学生（POST请求）
    if (request.find("POST /api/students") != std::string::npos) {
        // 从请求中提取参数
        size_t pos = request.find("\r\n\r\n");
        if (pos != std::string::npos) {
            std::string body = request.substr(pos + 4);
            
            // 解析参数 (id=123&name=张三&score1=90&score2=85&score3=95)
            std::string id, name;
            double s1=0, s2=0, s3=0;
            
            size_t start = 0;
            while (start < body.length()) {
                size_t end = body.find('&', start);
                if (end == std::string::npos) end = body.length();
                
                std::string pair = body.substr(start, end - start);
                size_t eq = pair.find('=');
                if (eq != std::string::npos) {
                    std::string key = pair.substr(0, eq);
                    std::string value = pair.substr(eq + 1);
                    
                    if (key == "id") id = value;
                    else if (key == "name") name = value;
                    else if (key == "score1") s1 = std::stod(value);
                    else if (key == "score2") s2 = std::stod(value);
                    else if (key == "score3") s3 = std::stod(value);
                }
                start = end + 1;
            }
            
            loadData();
            students.push_back(Student(id, name, s1, s2, s3));
            saveData();
            
            std::string response = "{\"success\":true}";
            return "HTTP/1.1 200 OK\r\n"
                   "Access-Control-Allow-Origin: *\r\n"
                   "Content-Type: application/json\r\n"
                   "Content-Length: " + std::to_string(response.length()) + "\r\n"
                   "\r\n" + response;
        }
    }
    
    // 如果是删除学生（DELETE请求）
    if (request.find("DELETE /api/students/") != std::string::npos) {
        size_t pos = request.find("/api/students/") + 14;
        size_t end = request.find(' ', pos);
        std::string id = request.substr(pos, end - pos);
        
        loadData();
        for (size_t i = 0; i < students.size(); i++) {
            if (students[i].id == id) {
                students.erase(students.begin() + i);
                saveData();
                break;
            }
        }
        
        std::string response = "{\"success\":true}";
        return "HTTP/1.1 200 OK\r\n"
               "Access-Control-Allow-Origin: *\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(response.length()) + "\r\n"
               "\r\n" + response;
    }
    
    // 如果是请求前端页面
    if (request.find("GET / ") != std::string::npos || request.find("GET /index.html") != std::string::npos) {
        std::ifstream file("../frontend/index.html");
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            return "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/html\r\n"
                   "Content-Length: " + std::to_string(content.length()) + "\r\n"
                   "\r\n" + content;
        } else {
            return "HTTP/1.1 404 Not Found\r\n\r\n<html><body><h1>index.html not found</h1></body></html>";
        }
    }
    
    // 默认返回
    return "HTTP/1.1 404 Not Found\r\n\r\n";
}

int main() {
    // 初始化Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }
    
    // 创建socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    
    // 绑定地址和端口
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    // 监听连接
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    std::cout << "=================================" << std::endl;
    std::cout << "服务器启动成功！" << std::endl;
    std::cout << "访问地址: http://localhost:8080" << std::endl;
    std::cout << "=================================" << std::endl;
    
    while (true) {
        // 接受客户端连接
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }
        
        // 接收请求
        char buffer[4096] = {0};
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            
            // 处理请求
            std::string response = handleRequest(buffer);
            
            // 发送响应
            send(clientSocket, response.c_str(), response.length(), 0);
        }
        
        // 关闭连接
        closesocket(clientSocket);
    }
    
    closesocket(serverSocket);
    WSACleanup();
    
    return 0;
}