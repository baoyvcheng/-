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
            cout << "�˳�����" << endl;
            break;
        default:
            cout << "��Ч��ѡ�����������롣" << endl;
            break;
        }
    } while (choice != '6');

    return 0;
}

void displayMenu() {
    cout << "\n===== ѧ������ϵͳ =====" << endl;
    cout << "1. ���ѧ����Ϣ" << endl;
    cout << "2. ����ѧ����Ϣ" << endl;
    cout << "3. �޸�ѧ����Ϣ" << endl;
    cout << "4. ɾ��ѧ����Ϣ" << endl;
    cout << "5. ���浽�ļ�" << endl;
    cout << "6. �˳�" << endl;
    cout << "������ѡ�";
}

void addStudent(vector<Student>& students) {
    Student newStudent;
    cout << "\n���ѧ����Ϣ��" << endl;
    cout << "������";
    getline(cin, newStudent.name);
    cout << "ѧ�ţ�";
    cin >> newStudent.id;
    cout << "GPA��";
    cin >> newStudent.gpa;
    students.push_back(newStudent);
    cout << "ѧ����Ϣ����ӡ�" << endl;
}

void findStudent(const vector<Student>& students) {
    int id;
    cout << "\n����ѧ����Ϣ��" << endl;
    cout << "������ѧ�ţ�";
    cin >> id;

    bool found = false;
    for (const auto& student : students) {
        if (student.id == id) {
            cout << "������" << student.name << endl;
            cout << "ѧ�ţ�" << student.id << endl;
            cout << "GPA��" << student.gpa << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "δ�ҵ���ѧ����" << endl;
    }
}

void updateStudent(vector<Student>& students) {
    int id;
    cout << "\n�޸�ѧ����Ϣ��" << endl;
    cout << "������Ҫ�޸���Ϣ��ѧ��ѧ�ţ�";
    cin >> id;

    bool found = false;
    for (auto& student : students) {
        if (student.id == id) {
            cout << "�������µ�������";
            getline(cin, student.name);
            cout << "�������µ�ѧ�ţ�";
            cin >> student.id;
            cout << "�������µ�GPA��";
            cin >> student.gpa;
            cout << "ѧ����Ϣ�Ѹ��¡�" << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "δ�ҵ���ѧ����" << endl;
    }
}

void deleteStudent(vector<Student>& students) {
    int id;
    cout << "\nɾ��ѧ����Ϣ��" << endl;
    cout << "������Ҫɾ����Ϣ��ѧ��ѧ�ţ�";
    cin >> id;

    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->id == id) {
            it = students.erase(it);
            cout << "ѧ����Ϣ��ɾ����" << endl;
            return;
        }
    }

    cout << "δ�ҵ���ѧ����" << endl;
}

void saveToFile(const vector<Student>& students) {
    ofstream outFile("students.txt");
    if (outFile.is_open()) {
        for (const auto& student : students) {
            outFile << student.name << ',' << student.id << ',' << student.gpa << endl;
        }
        outFile.close();
        cout << "ѧ����Ϣ�ѱ��浽�ļ���" << endl;
    }
    else {
        cout << "�޷����ļ����б��档" << endl;
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
        cout << "ѧ����Ϣ�Ѵ��ļ����ء�" << endl;
    }
    else {
        cout << "�޷����ļ����м��ء�" << endl;
    }
}
