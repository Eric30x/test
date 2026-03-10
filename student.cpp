#include "../include/student.h"

// 默认构造函数
Student::Student() {}

// 带参数的构造函数
Student::Student(const std::string& id, const std::string& name, 
                 const std::vector<double>& scores) 
    : id(id), name(name), scores(scores) {}

// 获取学号
std::string Student::getId() const { return id; }

// 获取姓名
std::string Student::getName() const { return name; }

// 获取成绩
std::vector<double> Student::getScores() const { return scores; }

// 计算总分
double Student::getTotal() const {
    double total = 0;
    for (double score : scores) {
        total += score;
    }
    return total;
}

// 计算平均分
double Student::getAverage() const {
    if (scores.empty()) return 0;
    return getTotal() / scores.size();
}

// 设置学号
void Student::setId(const std::string& newId) { id = newId; }

// 设置姓名
void Student::setName(const std::string& newName) { name = newName; }

// 设置成绩
void Student::setScores(const std::vector<double>& newScores) { 
    scores = newScores; 
}

// 转换为JSON格式
std::string Student::toJson() const {
    std::stringstream ss;
    ss << "{";
    ss << "\"id\":\"" << id << "\",";
    ss << "\"name\":\"" << name << "\",";
    ss << "\"scores\":[";
    for (size_t i = 0; i < scores.size(); ++i) {
        if (i > 0) ss << ",";
        ss << std::fixed << std::setprecision(1) << scores[i];
    }
    ss << "],";
    ss << "\"total\":" << std::fixed << std::setprecision(1) << getTotal() << ",";
    ss << "\"average\":" << std::fixed << std::setprecision(1) << getAverage();
    ss << "}";
    return ss.str();
}

// 从字符串解析学生（用于从文件读取）
Student Student::fromString(const std::string& line) {
    std::stringstream ss(line);
    std::string id, name;
    std::vector<double> scores;
    double score;
    
    ss >> id >> name;
    while (ss >> score) {
        scores.push_back(score);
    }
    
    return Student(id, name, scores);
}

// 转换为字符串（用于保存到文件）
std::string Student::toString() const {
    std::stringstream ss;
    ss << id << " " << name;
    for (double score : scores) {
        ss << " " << std::fixed << std::setprecision(1) << score;
    }
    return ss.str();
}