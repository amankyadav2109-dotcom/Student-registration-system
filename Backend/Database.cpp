#include "Database.h"
#include <iostream>
#include <vector>

using namespace std;


// ==========================================
// CONSTRUCTOR
// ==========================================

Database::Database()
{
    db = nullptr;
}


// ==========================================
// DESTRUCTOR
// ==========================================

Database::~Database()
{
    if (db != nullptr)
    {
        sqlite3_close(db);
    }
}


// ==========================================
// CONNECT TO DATABASE
// ==========================================

bool Database::connect()
{
    int result = sqlite3_open(
        "../database/student_registration.db",
        &db
    );

    if (result != SQLITE_OK)
    {
        cout << "Failed to connect to database!"
             << endl;

        return false;
    }

    cout << "Database connected successfully!"
         << endl;

    return true;
}


// ==========================================
// CREATE TABLE
// ==========================================

bool Database::createTable()
{
    string sql =
        "CREATE TABLE IF NOT EXISTS students ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "student_id INTEGER UNIQUE NOT NULL,"
        "name TEXT NOT NULL,"
        "email TEXT NOT NULL,"
        "phone TEXT NOT NULL,"
        "date_of_birth TEXT NOT NULL,"
        "course TEXT NOT NULL,"
        "semester INTEGER NOT NULL"
        ");";


    char* errorMessage = nullptr;


    int result = sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );


    if (result != SQLITE_OK)
    {
        cout << "Failed to create table!"
             << endl;

        cout << errorMessage
             << endl;

        sqlite3_free(errorMessage);

        return false;
    }


    cout << "Students table ready!"
         << endl;

    return true;
}


// ==========================================
// ADD STUDENT
// ==========================================

bool Database::addStudent(
    const Student& student
)
{
    string sql =
        "INSERT INTO students "
        "(student_id, name, email, phone, date_of_birth, course, semester) "
        "VALUES (" +

        to_string(
            student.getStudentId()
        ) +

        ", '" +

        student.getName() +

        "', '" +

        student.getEmail() +

        "', '" +

        student.getPhone() +

        "', '" +

        student.getDateOfBirth() +

        "', '" +

        student.getCourse() +

        "', " +

        to_string(
            student.getSemester()
        ) +

        ");";


    char* errorMessage = nullptr;


    int result = sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );


    if (result != SQLITE_OK)
    {
        cout << "Failed to add student!"
             << endl;

        cout << errorMessage
             << endl;

        sqlite3_free(errorMessage);

        return false;
    }


    cout << "Student added successfully!"
         << endl;

    return true;
}


// ==========================================
// UPDATE STUDENT
// ==========================================

bool Database::updateStudent(
    const Student& student
)
{
    string sql =
        "UPDATE students SET "

        "name = '" +
        student.getName() +

        "', email = '" +
        student.getEmail() +

        "', phone = '" +
        student.getPhone() +

        "', date_of_birth = '" +
        student.getDateOfBirth() +

        "', course = '" +
        student.getCourse() +

        "', semester = " +
        to_string(
            student.getSemester()
        ) +

        " WHERE student_id = " +
        to_string(
            student.getStudentId()
        ) +

        ";";


    char* errorMessage = nullptr;


    int result = sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );


    if (result != SQLITE_OK)
    {
        cout << "Failed to update student!"
             << endl;

        cout << errorMessage
             << endl;

        sqlite3_free(errorMessage);

        return false;
    }


    if (sqlite3_changes(db) == 0)
    {
        cout << "Student not found!"
             << endl;

        return false;
    }


    cout << "Student updated successfully!"
         << endl;

    return true;
}


// ==========================================
// DELETE STUDENT
// ==========================================

bool Database::deleteStudent(
    int student_id
)
{
    string sql =
        "DELETE FROM students "
        "WHERE student_id = " +

        to_string(student_id) +

        ";";


    char* errorMessage = nullptr;


    int result = sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );


    if (result != SQLITE_OK)
    {
        cout << "Failed to delete student!"
             << endl;

        cout << errorMessage
             << endl;

        sqlite3_free(errorMessage);

        return false;
    }


    if (sqlite3_changes(db) == 0)
    {
        cout << "Student not found!"
             << endl;

        return false;
    }


    cout << "Student deleted successfully!"
         << endl;

    return true;
}


// ==========================================
// GET ALL STUDENTS
// ==========================================

vector<Student> Database::getAllStudents()
{
    vector<Student> students;


    string sql =
        "SELECT student_id, name, email, phone, "
        "date_of_birth, course, semester "
        "FROM students;";


    sqlite3_stmt* statement;


    int result = sqlite3_prepare_v2(
        db,
        sql.c_str(),
        -1,
        &statement,
        nullptr
    );


    if (result != SQLITE_OK)
    {
        cout << "Failed to read students!"
             << endl;

        return students;
    }


    while (
        sqlite3_step(statement)
        == SQLITE_ROW
    )
    {
        int student_id =
            sqlite3_column_int(
                statement,
                0
            );


        string name =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    statement,
                    1
                )
            );


        string email =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    statement,
                    2
                )
            );


        string phone =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    statement,
                    3
                )
            );


        string date_of_birth =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    statement,
                    4
                )
            );


        string course =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    statement,
                    5
                )
            );


        int semester =
            sqlite3_column_int(
                statement,
                6
            );


        Student student(
            student_id,
            name,
            email,
            phone,
            date_of_birth,
            course,
            semester
        );


        students.push_back(
            student
        );
    }


    sqlite3_finalize(
        statement
    );


    return students;
}