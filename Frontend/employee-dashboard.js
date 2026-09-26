const API_URL = "http://localhost:8080";

let students = [];


// ==========================================
// GET HTML ELEMENTS
// ==========================================

const studentTableBody =
    document.getElementById("studentTableBody");

const totalStudents =
    document.getElementById("totalStudents");

const registeredStudents =
    document.getElementById("registeredStudents");

const searchInput =
    document.getElementById("searchInput");

const sortSelect =
    document.getElementById("sortSelect");

const refreshButton =
    document.getElementById("refreshButton");

const noStudentsMessage =
    document.getElementById("noStudentsMessage");

const logoutButton =
    document.getElementById("logoutButton");


// ==========================================
// LOAD STUDENTS
// ==========================================

async function loadStudents()
{
    studentTableBody.innerHTML =
        `<tr>
            <td colspan="8">
                Loading student records...
            </td>
        </tr>`;


    try
    {
        const response =
            await fetch(
                `${API_URL}/students`
            );


        if (!response.ok)
        {
            throw new Error(
                "Unable to fetch student records."
            );
        }


        students =
            await response.json();


        updateStudentCounts();

        displayStudents();
    }
    catch (error)
    {
        console.error(
            "Error:",
            error
        );


        studentTableBody.innerHTML =
            `<tr>
                <td colspan="8">
                    Could not connect to the backend.
                    Make sure server.exe is running.
                </td>
            </tr>`;


        noStudentsMessage.style.display =
            "none";


        totalStudents.textContent =
            "0";


        registeredStudents.textContent =
            "0";
    }
}


// ==========================================
// UPDATE COUNTERS
// ==========================================

function updateStudentCounts()
{
    totalStudents.textContent =
        students.length;


    registeredStudents.textContent =
        students.length;
}


// ==========================================
// DISPLAY STUDENTS
// ==========================================

function displayStudents()
{
    const searchText =
        searchInput.value
            .toLowerCase()
            .trim();


    const sortBy =
        sortSelect.value;


    let filteredStudents =
        [...students];


    // SEARCH
    if (searchText !== "")
    {
        filteredStudents =
            filteredStudents.filter(
                function(student)
                {
                    return (
                        String(student.student_id)
                            .toLowerCase()
                            .includes(searchText)

                        ||

                        student.name
                            .toLowerCase()
                            .includes(searchText)

                        ||

                        student.email
                            .toLowerCase()
                            .includes(searchText)

                        ||

                        student.phone
                            .toLowerCase()
                            .includes(searchText)

                        ||

                        student.date_of_birth
                            .toLowerCase()
                            .includes(searchText)

                        ||

                        student.course
                            .toLowerCase()
                            .includes(searchText)

                        ||

                        String(student.semester)
                            .includes(searchText)
                    );
                }
            );
    }


    // SORT
    if (
        sortBy === "student_id" ||
        sortBy === "semester"
    )
    {
        filteredStudents.sort(
            function(a, b)
            {
                return (
                    Number(a[sortBy]) -
                    Number(b[sortBy])
                );
            }
        );
    }


    else if (
        sortBy === "name" ||
        sortBy === "course"
    )
    {
        filteredStudents.sort(
            function(a, b)
            {
                return a[sortBy]
                    .localeCompare(
                        b[sortBy]
                    );
            }
        );
    }


    studentTableBody.innerHTML = "";


    // NO STUDENTS
    if (filteredStudents.length === 0)
    {
        noStudentsMessage.style.display =
            "block";


        if (students.length === 0)
        {
            noStudentsMessage.textContent =
                "No students registered yet.";
        }
        else
        {
            noStudentsMessage.textContent =
                "No students match your search.";
        }


        return;
    }


    noStudentsMessage.style.display =
        "none";


    // CREATE ROWS
    filteredStudents.forEach(
        function(student)
        {
            const row =
                document.createElement("tr");


            const values = [
                student.student_id,
                student.name,
                student.email,
                student.phone,
                student.date_of_birth,
                student.course,
                student.semester
            ];


            values.forEach(
                function(value)
                {
                    const cell =
                        document.createElement("td");


                    cell.textContent =
                        value ?? "";


                    row.appendChild(
                        cell
                    );
                }
            );


            // ==========================================
            // ACTION CELL
            // ==========================================

            const actionCell =
                document.createElement("td");


            // ==========================================
            // UPDATE BUTTON
            // ==========================================

            const updateButton =
                document.createElement("button");


            updateButton.textContent =
                "Update";


            updateButton.className =
                "action-button update-button";


            updateButton.addEventListener(
                "click",
                function()
                {
                    showUpdateForm(student);
                }
            );


            // ==========================================
            // DELETE BUTTON
            // ==========================================

            const deleteButton =
                document.createElement("button");


            deleteButton.textContent =
                "Delete";


            deleteButton.className =
                "action-button delete-button";


            deleteButton.addEventListener(
                "click",
                async function()
                {
                    const confirmDelete =
                        confirm(
                            `Are you sure you want to delete ${student.name}?`
                        );


                    if (!confirmDelete)
                    {
                        return;
                    }


                    try
                    {
                        const response =
                            await fetch(
                                `${API_URL}/students/${student.student_id}`,
                                {
                                    method: "DELETE"
                                }
                            );


                        const result =
                            await response.json();


                        if (!response.ok)
                        {
                            alert(
                                result.message ||
                                "Failed to delete student."
                            );

                            return;
                        }


                        alert(
                            result.message
                        );


                        loadStudents();
                    }
                    catch (error)
                    {
                        console.error(
                            "Delete error:",
                            error
                        );


                        alert(
                            "Could not connect to the backend. Make sure server.exe is running."
                        );
                    }
                }
            );


            actionCell.appendChild(
                updateButton
            );


            actionCell.appendChild(
                deleteButton
            );


            row.appendChild(
                actionCell
            );


            studentTableBody.appendChild(
                row
            );
        }
    );
}


// ==========================================
// SHOW UPDATE FORM
// ==========================================

function showUpdateForm(student)
{
    const overlay =
        document.createElement("div");


    overlay.id =
        "updateOverlay";


    overlay.style.position =
        "fixed";

    overlay.style.top =
        "0";

    overlay.style.left =
        "0";

    overlay.style.width =
        "100%";

    overlay.style.height =
        "100%";

    overlay.style.background =
        "rgba(0, 0, 0, 0.55)";

    overlay.style.display =
        "flex";

    overlay.style.alignItems =
        "center";

    overlay.style.justifyContent =
        "center";

    overlay.style.zIndex =
        "9999";

    overlay.style.padding =
        "20px";


    const formBox =
        document.createElement("div");


    formBox.style.background =
        "white";

    formBox.style.padding =
        "30px";

    formBox.style.borderRadius =
        "18px";

    formBox.style.width =
        "100%";

    formBox.style.maxWidth =
        "520px";

    formBox.style.maxHeight =
        "90vh";

    formBox.style.overflowY =
        "auto";


    formBox.innerHTML = `
        <h2 style="margin-top: 0;">
            Update Student
        </h2>

        <p style="color: #666;">
            Student ID: <strong>${student.student_id}</strong>
        </p>

        <label>Name</label>

        <input
            id="editName"
            type="text"
            value="${escapeHTML(student.name)}"
            style="
                width:100%;
                padding:10px;
                margin:6px 0 15px;
                box-sizing:border-box;
            "
        >

        <label>Email</label>

        <input
            id="editEmail"
            type="email"
            value="${escapeHTML(student.email)}"
            style="
                width:100%;
                padding:10px;
                margin:6px 0 15px;
                box-sizing:border-box;
            "
        >

        <label>Phone</label>

        <input
            id="editPhone"
            type="text"
            value="${escapeHTML(student.phone)}"
            style="
                width:100%;
                padding:10px;
                margin:6px 0 15px;
                box-sizing:border-box;
            "
        >

        <label>Date of Birth</label>

        <input
            id="editDob"
            type="date"
            value="${escapeHTML(student.date_of_birth)}"
            style="
                width:100%;
                padding:10px;
                margin:6px 0 15px;
                box-sizing:border-box;
            "
        >

        <label>Course</label>

        <input
            id="editCourse"
            type="text"
            value="${escapeHTML(student.course)}"
            style="
                width:100%;
                padding:10px;
                margin:6px 0 15px;
                box-sizing:border-box;
            "
        >

        <label>Semester</label>

        <input
            id="editSemester"
            type="number"
            min="1"
            value="${student.semester}"
            style="
                width:100%;
                padding:10px;
                margin:6px 0 20px;
                box-sizing:border-box;
            "
        >

        <div style="
            display:flex;
            gap:10px;
            justify-content:flex-end;
        ">

            <button
                id="cancelUpdate"
                type="button"
                style="
                    padding:10px 18px;
                    border:none;
                    border-radius:8px;
                    cursor:pointer;
                "
            >
                Cancel
            </button>

            <button
                id="saveUpdate"
                type="button"
                style="
                    padding:10px 18px;
                    border:none;
                    border-radius:8px;
                    cursor:pointer;
                    background:#2563eb;
                    color:white;
                "
            >
                Save Changes
            </button>

        </div>
    `;


    overlay.appendChild(
        formBox
    );


    document.body.appendChild(
        overlay
    );


    // CANCEL
    document
        .getElementById("cancelUpdate")
        .addEventListener(
            "click",
            function()
            {
                overlay.remove();
            }
        );


    // SAVE
    document
        .getElementById("saveUpdate")
        .addEventListener(
            "click",
            function()
            {
                updateStudent(
                    student.student_id,
                    overlay
                );
            }
        );
}


// ==========================================
// UPDATE STUDENT
// ==========================================

async function updateStudent(
    studentId,
    overlay
)
{
    const name =
        document.getElementById(
            "editName"
        ).value.trim();


    const email =
        document.getElementById(
            "editEmail"
        ).value.trim();


    const phone =
        document.getElementById(
            "editPhone"
        ).value.trim();


    const date_of_birth =
        document.getElementById(
            "editDob"
        ).value;


    const course =
        document.getElementById(
            "editCourse"
        ).value.trim();


    const semester =
        document.getElementById(
            "editSemester"
        ).value;


    if (
        name === "" ||
        email === "" ||
        phone === "" ||
        date_of_birth === "" ||
        course === "" ||
        semester === ""
    )
    {
        alert(
            "Please fill all fields."
        );

        return;
    }


    const updatedStudent =
    {
        name: name,
        email: email,
        phone: phone,
        date_of_birth: date_of_birth,
        course: course,
        semester: Number(semester)
    };


    try
    {
        const response =
            await fetch(
                `${API_URL}/students/${studentId}`,
                {
                    method: "PUT",

                    headers:
                    {
                        "Content-Type":
                            "application/json"
                    },

                    body:
                        JSON.stringify(
                            updatedStudent
                        )
                }
            );


        const result =
            await response.json();


        if (!response.ok)
        {
            alert(
                result.message ||
                "Failed to update student."
            );

            return;
        }


        alert(
            result.message
        );


        overlay.remove();


        loadStudents();
    }
    catch (error)
    {
        console.error(
            "Update error:",
            error
        );


        alert(
            "Could not connect to the backend. Make sure server.exe is running."
        );
    }
}


// ==========================================
// ESCAPE HTML
// ==========================================

function escapeHTML(value)
{
    return String(value)
        .replace(
            /&/g,
            "&amp;"
        )
        .replace(
            /</g,
            "&lt;"
        )
        .replace(
            />/g,
            "&gt;"
        )
        .replace(
            /"/g,
            "&quot;"
        )
        .replace(
            /'/g,
            "&#039;"
        );
}


// ==========================================
// SEARCH
// ==========================================

searchInput.addEventListener(
    "input",
    displayStudents
);


// ==========================================
// SORT
// ==========================================

sortSelect.addEventListener(
    "change",
    displayStudents
);


// ==========================================
// REFRESH
// ==========================================

refreshButton.addEventListener(
    "click",
    loadStudents
);


// ==========================================
// LOGOUT
// ==========================================

logoutButton.addEventListener(
    "click",
    function()
    {
        window.location.href =
            "index.html";
    }
);


// ==========================================
// INITIAL LOAD
// ==========================================

loadStudents();