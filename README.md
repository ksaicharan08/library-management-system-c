# Library Management System

A small console project based on the provided Book Management Module specification.
I kept the program with arrays, structures, functions, and text files so each part is easy to read and explain.

## Features

- Add, update, remove, search, and list books
- Issue a book for 7 days
- Return a book and calculate a fine of Rs. 5 for each late day
- List current and completed issue records
- Save books and issue history to text files
- Load saved data when the program starts
- Reject negative quantities and invalid whole-number input

## Project structure

```text
Library Management System_K5/
|-- include/
|   `-- library.h       Shared structures, constants, and function declarations
|-- src/
|   |-- main.c          Main menu and program starting point
|   |-- books.c         Add, update, remove, search, and display books
|   |-- issues.c        Issue, return, fine, and issue-history features
|   |-- input.c         Safe keyboard input functions
|   `-- date.c          Today, due-date, and late-day calculations
|-- data/
|   |-- books.txt       Saved book records
|   `-- issues.txt      Saved issue and return records
`-- README.md
```

## How I explain the program

1. `main.c` creates one array for books and one array for issue records.
2. The load functions read saved lines from the `data` folder.
3. The user selects an option from the menu.
4. `books.c` handles book work, while `issues.c` handles borrowing work.
5. Save or Exit writes the current arrays back to the text files.

The header file is shared by all source files. This allows every module to use the same `Book` and `IssueRecord` structures without copying their definitions.

## Record formats

The files use the pipe character (`|`) between fields because it is easy to read:

```text
books.txt:  bookId|title|author|quantity
issues.txt: issueId|bookId|userId|userName|issueDate|dueDate|returnDate|fine|returned
```

Do not type `|` inside a title, author name, or user name because it is the field separator.

## Build on Windows

Install a C compiler such as MinGW-w64 or use the MSYS2 terminal. Then open a terminal in this project folder and run:

```text
gcc -Wall -Wextra -std=c11 -Iinclude src/main.c src/books.c src/issues.c src/input.c src/date.c -o library.exe
library.exe
```

Run the command from the project root. The program expects the `data` folder to be the current folder.

With Microsoft Visual C, the equivalent idea is:

```text
cl /W4 /Iinclude src\main.c src\books.c src\issues.c src\input.c src\date.c /Fe:library.exe
library.exe
```

## Easy test sequence

1. Choose `1` for Book management.
2. Choose `1` and add book ID `101`, title `C Programming`, author `Dennis Ritchie`, quantity `2`.
3. Choose `5` to confirm the book appears.
4. Return to the main menu and choose `2` to issue book `101` to user ID `7`.
5. Choose `4` to view the issue record and its due date.
6. Choose `3` to return the book using book ID `101` and user ID `7`.
7. Choose `5` to save, then exit and start the program again.
8. The book and its issue history should still be present.

## Short explanation for presentation

A `struct` groups related values. My `Book` structure stores an ID, title, author, and quantity. My `IssueRecord` structure stores one borrowing action.

The arrays hold many structures. For example, `books[0]` is the first book. `bookCount` tells the program how many positions are in use.

`find_book_by_id` searches the book array and returns the matching book. The pointer lets another function update that book directly.

When a book is issued, its quantity goes down by one and a new issue record is added. When it is returned, the quantity goes up by one. The date module compares the return date with the due date and multiplies late days by `FINE_PER_DAY`.

I chose fixed limits (`100` books and `200` issue records) because they avoid dynamic memory while I am learning. A future version could use linked lists or dynamic memory.

The project is intentionally my own small design: a book is due after 7 days, the fine is Rs. 5 per late day, and the files use `|` separators. These choices are simple to demonstrate and easy to change in `library.h` or `issues.c`.

## Important limitations kept simple

- Search is partial but case-sensitive. For example, `C` finds `C Programming`, but `c` may not.
- User names are stored in issue records; there is no separate user-management module because the supplied specification only requires a user ID and name during issuing.
- Data files are plain text, which is excellent for learning but not suitable for multi-user production software.
