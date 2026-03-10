#ifndef DATABASE_H
#define DATABASE_H

#include "student.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class Database {
private:
    std::vector<Student> students;
    std::string filename;
    
    void loadFromFile();
    void saveToFile();

public:
    Database(const std::string& fname);
    
    std::vector<Student> getAllStudents();
    Student* findStudent(const std::string& id);
    bool addStudent(const Student& student);
    bool updateStudent(const std::string& id, const Student& newStudent);
    bool deleteStudent(const std::string& id);
    std::string getAllStudentsJson();
};

#endif