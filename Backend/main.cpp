#include <iostream>
#include <vector>

#include "Student.h"
#include "Database.h"
#include "httplib.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


int main()
{
    httplib::Server server;


    // ==========================================
    // CORS
    // ==========================================

    server.set_pre_routing_handler(
        [](const httplib::Request& req,
           httplib::Response& res)
        {
            res.set_header(
                "Access-Control-Allow-Origin",
                "*"
            );

            res.set_header(
                "Access-Control-Allow-Methods",
                "GET, POST, PUT, DELETE, OPTIONS"
            );

            res.set_header(
                "Access-Control-Allow-Headers",
                "Content-Type"
            );


            if (req.method == "OPTIONS")
            {
                res.status = 200;

                return httplib::Server::HandlerResponse::Handled;
            }


            return httplib::Server::HandlerResponse::Unhandled;
        }
    );


    // ==========================================
    // TEST ROUTE
    // ==========================================

    server.Get(
        "/",
        [](const httplib::Request& req,
           httplib::Response& res)
        {
            res.set_content(
                "C++ Backend is working!",
                "text/plain"
            );
        }
    );


    // ==========================================
    // REGISTER STUDENT
    // ==========================================

    server.Post(
        "/register",
        [](const httplib::Request& req,
           httplib::Response& res)
        {
            cout << endl;
            cout << "Registration request received!"
                 << endl;


            try
            {
                json data =
                    json::parse(req.body);


                int student_id =
                    stoi(
                        data["student_id"]
                            .get<string>()
                    );


                string name =
                    data["name"]
                        .get<string>();


                string email =
                    data["email"]
                        .get<string>();


                string phone =
                    data["phone"]
                        .get<string>();


                string date_of_birth =
                    data["date_of_birth"]
                        .get<string>();


                string course =
                    data["course"]
                        .get<string>();


                int semester =
                    stoi(
                        data["semester"]
                            .get<string>()
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


                Database database;


                if (!database.connect())
                {
                    res.status = 500;

                    res.set_content(
                        "Database connection failed!",
                        "text/plain"
                    );

                    return;
                }


                if (!database.createTable())
                {
                    res.status = 500;

                    res.set_content(
                        "Could not create database table!",
                        "text/plain"
                    );

                    return;
                }


                if (!database.addStudent(student))
                {
                    res.status = 400;

                    res.set_content(
                        "Could not register student!",
                        "text/plain"
                    );

                    return;
                }


                cout << "Student ID: "
                     << student_id
                     << endl;

                cout << "Name: "
                     << name
                     << endl;

                cout << "Email: "
                     << email
                     << endl;

                cout << "Phone: "
                     << phone
                     << endl;

                cout << "Date of Birth: "
                     << date_of_birth
                     << endl;

                cout << "Course: "
                     << course
                     << endl;

                cout << "Semester: "
                     << semester
                     << endl;


                res.set_content(
                    "Student registered successfully!",
                    "text/plain"
                );
            }
            catch (...)
            {
                res.status = 400;

                res.set_content(
                    "Invalid student data!",
                    "text/plain"
                );
            }
        }
    );


    // ==========================================
    // GET ALL STUDENTS
    // ==========================================

    server.Get(
        "/students",
        [](const httplib::Request& req,
           httplib::Response& res)
        {
            cout << endl;
            cout << "Request to get students received!"
                 << endl;


            Database database;


            if (!database.connect())
            {
                res.status = 500;

                res.set_content(
                    "Database connection failed!",
                    "text/plain"
                );

                return;
            }


            if (!database.createTable())
            {
                res.status = 500;

                res.set_content(
                    "Could not create database table!",
                    "text/plain"
                );

                return;
            }


            vector<Student> students =
                database.getAllStudents();


            json response =
                json::array();


            for (const Student& student : students)
            {
                json studentData =
                {
                    {
                        "student_id",
                        student.getStudentId()
                    },

                    {
                        "name",
                        student.getName()
                    },

                    {
                        "email",
                        student.getEmail()
                    },

                    {
                        "phone",
                        student.getPhone()
                    },

                    {
                        "date_of_birth",
                        student.getDateOfBirth()
                    },

                    {
                        "course",
                        student.getCourse()
                    },

                    {
                        "semester",
                        student.getSemester()
                    }
                };


                response.push_back(
                    studentData
                );
            }


            res.set_content(
                response.dump(),
                "application/json"
            );
        }
    );


    // ==========================================
    // UPDATE STUDENT
    // ==========================================

    server.Put(
        R"(/students/(\d+))",
        [](const httplib::Request& req,
           httplib::Response& res)
        {
            cout << endl;
            cout << "Update request received!"
                 << endl;


            try
            {
                int student_id =
                    stoi(
                        req.matches[1].str()
                    );


                json data =
                    json::parse(req.body);


                string name =
                    data["name"]
                        .get<string>();


                string email =
                    data["email"]
                        .get<string>();


                string phone =
                    data["phone"]
                        .get<string>();


                string date_of_birth =
                    data["date_of_birth"]
                        .get<string>();


                string course =
                    data["course"]
                        .get<string>();


                int semester =
                    data["semester"]
                        .get<int>();


                Student student(
                    student_id,
                    name,
                    email,
                    phone,
                    date_of_birth,
                    course,
                    semester
                );


                Database database;


                if (!database.connect())
                {
                    res.status = 500;

                    res.set_content(
                        R"({"success":false,"message":"Database connection failed!"})",
                        "application/json"
                    );

                    return;
                }


                if (!database.updateStudent(student))
                {
                    res.status = 404;

                    res.set_content(
                        R"({"success":false,"message":"Student not found!"})",
                        "application/json"
                    );

                    return;
                }


                cout << "Student ID "
                     << student_id
                     << " updated successfully!"
                     << endl;


                res.status = 200;


                res.set_content(
                    R"({"success":true,"message":"Student updated successfully!"})",
                    "application/json"
                );
            }
            catch (...)
            {
                res.status = 400;

                res.set_content(
                    R"({"success":false,"message":"Invalid student data!"})",
                    "application/json"
                );
            }
        }
    );


    // ==========================================
    // DELETE STUDENT
    // ==========================================

    server.Delete(
        R"(/students/(\d+))",
        [](const httplib::Request& req,
           httplib::Response& res)
        {
            cout << endl;
            cout << "Delete request received!"
                 << endl;


            try
            {
                int student_id =
                    stoi(
                        req.matches[1].str()
                    );


                cout << "Deleting Student ID: "
                     << student_id
                     << endl;


                Database database;


                if (!database.connect())
                {
                    res.status = 500;

                    res.set_content(
                        R"({"success":false,"message":"Database connection failed!"})",
                        "application/json"
                    );

                    return;
                }


                if (!database.deleteStudent(student_id))
                {
                    res.status = 404;

                    res.set_content(
                        R"({"success":false,"message":"Student not found!"})",
                        "application/json"
                    );

                    return;
                }


                res.status = 200;


                res.set_content(
                    R"({"success":true,"message":"Student deleted successfully!"})",
                    "application/json"
                );
            }
            catch (...)
            {
                res.status = 400;

                res.set_content(
                    R"({"success":false,"message":"Invalid student ID!"})",
                    "application/json"
                );
            }
        }
    );


    // ==========================================
    // START SERVER
    // ==========================================

    cout << endl;

    cout << "===================================="
         << endl;

    cout << "Server started at:"
         << endl;

    cout << "http://localhost:8080"
         << endl;

    cout << "===================================="
         << endl;


    server.listen(
        "localhost",
        8080
    );


    return 0;
}