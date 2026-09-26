#include "Student.h"

Student::Student(
    int student_id,
    string name,
    string email,
    string phone,
    string date_of_birth,
    string course,
    int semester
)
{
    this->student_id = student_id;
    this->name = name;
    this->email = email;
    this->phone = phone;
    this->date_of_birth = date_of_birth;
    this->course = course;
    this->semester = semester;
}

int Student::getStudentId() const
{
    return student_id;
}

string Student::getName() const
{
    return name;
}

string Student::getEmail() const
{
    return email;
}

string Student::getPhone() const
{
    return phone;
}

string Student::getDateOfBirth() const
{
    return date_of_birth;
}

string Student::getCourse() const
{
    return course;
}

int Student::getSemester() const
{
    return semester;
}