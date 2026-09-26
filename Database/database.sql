CREATE TABLE IF NOT EXISTS students (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    student_id INTEGER UNIQUE NOT NULL,
    name TEXT NOT NULL,
    email TEXT NOT NULL,
    phone TEXT NOT NULL,
    date_of_birth TEXT NOT NULL,
    course TEXT NOT NULL,
    semester INTEGER NOT NULL
);