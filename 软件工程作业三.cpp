#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

struct Student {
    string name;
    int id;
    float gpa;
};

// Function prototypes
void displayMenu();
void addStudent(vector<Student>& students);
void findStudent(const vector<Student>& students);
void updateStudent(vector<Student>& students);
void deleteStudent(vector<Student>& students);
void saveToFile(const vector<Student>& students);
void loadFromFile(vector<Student>& students);

int main() {
    vector<Student> students;
    loadFromFile(students);

    char choice;
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case '1':
            addStudent(students);
            break;
        case '2':
            findStudent(students);
            break;
        case '3':
            updateStudent(students);
            break;
        case '4':
            deleteStudent(students);
            break;
        case '5':
            saveToFile(students);
            break;
        case '6':
            cout << "退出程序。" << endl;
            break;
        default:
            cout << "无效的选择，请重新输入。" << endl;
            break;
        }
    } while (choice != '6');

    return 0;
}

void displayMenu() {
    cout << "\n===== 学生管理系统 =====" << endl;
    cout << "1. 添加学生信息" << endl;
    cout << "2. 查找学生信息" << endl;
    cout << "3. 修改学生信息" << endl;
    cout << "4. 删除学生信息" << endl;
    cout << "5. 保存到文件" << endl;
    cout << "6. 退出" << endl;
    cout << "请输入选项：";
}

void addStudent(vector<Student>& students) {
    Student newStudent;
    cout << "\n添加学生信息：" << endl;
    cout << "姓名：";
    getline(cin, newStudent.name);
    cout << "学号：";
    cin >> newStudent.id;
    cout << "GPA：";
    cin >> newStudent.gpa;
    students.push_back(newStudent);
    cout << "学生信息已添加。" << endl;
}

void findStudent(const vector<Student>& students) {
    int id;
    cout << "\n查找学生信息：" << endl;
    cout << "请输入学号：";
    cin >> id;

    bool found = false;
    for (const auto& student : students) {
        if (student.id == id) {
            cout << "姓名：" << student.name << endl;
            cout << "学号：" << student.id << endl;
            cout << "GPA：" << student.gpa << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "未找到该学生。" << endl;
    }
}

void updateStudent(vector<Student>& students) {
    int id;
    cout << "\n修改学生信息：" << endl;
    cout << "请输入要修改信息的学生学号：";
    cin >> id;

    bool found = false;
    for (auto& student : students) {
        if (student.id == id) {
            cout << "请输入新的姓名：";
            getline(cin, student.name);
            cout << "请输入新的学号：";
            cin >> student.id;
            cout << "请输入新的GPA：";
            cin >> student.gpa;
            cout << "学生信息已更新。" << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "未找到该学生。" << endl;
    }
}

void deleteStudent(vector<Student>& students) {
    int id;
    cout << "\n删除学生信息：" << endl;
    cout << "请输入要删除信息的学生学号：";
    cin >> id;

    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->id == id) {
            it = students.erase(it);
            cout << "学生信息已删除。" << endl;
            return;
        }
    }

    cout << "未找到该学生。" << endl;
}

void saveToFile(const vector<Student>& students) {
    ofstream outFile("students.txt");
    if (outFile.is_open()) {
        for (const auto& student : students) {
            outFile << student.name << ',' << student.id << ',' << student.gpa << endl;
        }
        outFile.close();
        cout << "学生信息已保存到文件。" << endl;
    }
    else {
        cout << "无法打开文件进行保存。" << endl;
    }
}

void loadFromFile(vector<Student>& students) {
    ifstream inFile("students.txt");
    if (inFile.is_open()) {
        while (!inFile.eof()) {
            Student newStudent;
            getline(inFile, newStudent.name, ',');
            inFile >> newStudent.id;
            inFile.ignore(numeric_limits<streamsize>::max(), ',');
            inFile >> newStudent.gpa;
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');
            students.push_back(newStudent);
        }
        inFile.close();
        cout << "学生信息已从文件加载。" << endl;
    }
    else {
        cout << "无法打开文件进行加载。" << endl;
    }
}
