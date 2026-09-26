#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include "Student.h"
#include <vector>

using namespace std;

class Database
{
private:
    sqlite3* db;

public:
    Database();
    ~Database();

    bool connect();
    bool createTable();

    bool addStudent(const Student& student);
    bool updateStudent(const Student& student);
    bool deleteStudent(int student_id);

    vector<Student> getAllStudents();
};

#endif