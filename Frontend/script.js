const form = document.getElementById("studentForm");

// ===============================
// REGISTER STUDENT
// ===============================

form.addEventListener("submit", async function(event)
{
    event.preventDefault();

    const studentData =
    {
        student_id: document.getElementById("student_id").value,
        name: document.getElementById("name").value,
        email: document.getElementById("email").value,
        phone: document.getElementById("phone").value,
        date_of_birth: document.getElementById("dob").value,
        course: document.getElementById("course").value,
        semester: document.getElementById("semester").value
    };

    try
    {
        const response = await fetch(
            "http://localhost:8080/register",
            {
                method: "POST",

                headers:
                {
                    "Content-Type": "application/json"
                },

                body: JSON.stringify(studentData)
            }
        );

        const result = await response.text();

        alert(result);

        if (response.ok)
        {
            form.reset();
        }
    }
    catch (error)
    {
        alert("Could not connect to C++ backend.");

        console.error(error);
    }
});


// ===============================
// LOAD REGISTERED STUDENTS
// ===============================

const loadStudentsButton =
    document.getElementById("loadStudents");

const studentsTable =
    document.getElementById("studentsTable");


loadStudentsButton.addEventListener(
    "click",
    async function()
    {
        try
        {
            const response = await fetch(
                "http://localhost:8080/students"
            );

            if (!response.ok)
            {
                throw new Error("Failed to load students.");
            }

            const students =
                await response.json();


            if (students.length === 0)
            {
                studentsTable.innerHTML =
                    "<p>No students registered yet.</p>";

                return;
            }


            let tableHTML = `
                <table>

                    <thead>

                        <tr>
                            <th>Student ID</th>
                            <th>Name</th>
                            <th>Email</th>
                            <th>Phone</th>
                            <th>Date of Birth</th>
                            <th>Course</th>
                            <th>Semester</th>
                        </tr>

                    </thead>

                    <tbody>
            `;


            students.forEach(function(student)
            {
                tableHTML += `
                    <tr>

                        <td>${student.student_id}</td>

                        <td>${student.name}</td>

                        <td>${student.email}</td>

                        <td>${student.phone}</td>

                        <td>${student.date_of_birth}</td>

                        <td>${student.course}</td>

                        <td>${student.semester}</td>

                    </tr>
                `;
            });


            tableHTML += `
                    </tbody>

                </table>
            `;


            studentsTable.innerHTML = tableHTML;
        }
        catch (error)
        {
            studentsTable.innerHTML =
                "<p>Could not load registered students.</p>";

            console.error(error);
        }
    }
);