#ifndef SERVER_H
#define SERVER_H

#include "database.h"
#include <string>

class StudentServer {
private:
    Database db;
    int port;

public:
    StudentServer(const std::string& dataFile, int p = 8080);
    void start();
};

#endif