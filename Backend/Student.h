#ifndef STUDENT_H
#define STUDENT_H

#include <string>

using namespace std;

class Student
{
private:
    int student_id;
    string name;
    string email;
    string phone;
    string date_of_birth;
    string course;
    int semester;

public:
    Student(
        int student_id,
        string name,
        string email,
        string phone,
        string date_of_birth,
        string course,
        int semester
    );

    int getStudentId() const;
string getName() const;
string getEmail() const;
string getPhone() const;
string getDateOfBirth() const;
string getCourse() const;
int getSemester() const;
};

#endif