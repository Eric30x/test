#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

class Student {
private:
    std::string id;           // 学号
    std::string name;         // 姓名
    std::vector<double> scores; // 各科成绩

public:
    // 构造函数
    Student();
    Student(const std::string& id, const std::string& name, 
            const std::vector<double>& scores);
    
    // 获取数据的函数
    std::string getId() const;
    std::string getName() const;
    std::vector<double> getScores() const;
    double getTotal() const;
    double getAverage() const;
    
    // 设置数据的函数
    void setId(const std::string& newId);
    void setName(const std::string& newName);
    void setScores(const std::vector<double>& newScores);
    
    // 其他功能
    std::string toJson() const;
    static Student fromString(const std::string& line);
    std::string toString() const;
};

#endif