#include "../include/database.h"
#include <iostream>

// 构造函数：加载数据
Database::Database(const std::string& fname) : filename(fname) {
    loadFromFile();
}

// 从文件加载数据
void Database::loadFromFile() {
    students.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            students.push_back(Student::fromString(line));
        }
    }
    file.close();
}

// 保存数据到文件
void Database::saveToFile() {
    std::ofstream file(filename);
    for (const auto& student : students) {
        file << student.toString() << std::endl;
    }
    file.close();
}

// 获取所有学生
std::vector<Student> Database::getAllStudents() {
    return students;
}

// 查找学生
Student* Database::findStudent(const std::string& id) {
    for (auto& student : students) {
        if (student.getId() == id) {
            return &student;
        }
    }
    return nullptr;
}

// 添加学生
bool Database::addStudent(const Student& student) {
    if (findStudent(student.getId()) != nullptr) {
        return false; // 学号已存在
    }
    students.push_back(student);
    saveToFile();
    return true;
}

// 更新学生
bool Database::updateStudent(const std::string& id, const Student& newStudent) {
    for (auto& student : students) {
        if (student.getId() == id) {
            student = newStudent;
            saveToFile();
            return true;
        }
    }
    return false;
}

// 删除学生
bool Database::deleteStudent(const std::string& id) {
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].getId() == id) {
            students.erase(students.begin() + i);
            saveToFile();
            return true;
        }
    }
    return false;
}

// 获取所有学生的JSON字符串
std::string Database::getAllStudentsJson() {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < students.size(); ++i) {
        if (i > 0) ss << ",";
        ss << students[i].toJson();
    }
    ss << "]";
    return ss.str();
}