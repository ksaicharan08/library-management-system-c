#include <stdio.h>
#include <string.h>

#include "library.h"

#define BOOKS_FILE "data/books.txt"

static void print_book(const Book *book) {
    printf("%-6d %-30s %-25s %d\n",
           book->id, book->title, book->author, book->quantity);
}

Book *find_book_by_id(Book books[], int bookCount, int id) {
    int index;

    for (index = 0; index < bookCount; index++) {
        if (books[index].id == id) {
            return &books[index];
        }
    }
    return NULL;
}

void load_books(Book books[], int *bookCount) {
    FILE *file = fopen(BOOKS_FILE, "r");
    char line[TEXT_SIZE * 3];
    Book book;

    *bookCount = 0;
    if (file == NULL) {
        return;
    }

    while (*bookCount < MAX_BOOKS && fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d|%99[^|]|%99[^|]|%d",
                   &book.id, book.title, book.author, &book.quantity) == 4) {
            books[*bookCount] = book;
            (*bookCount)++;
        }
    }
    fclose(file);
}

void save_books(const Book books[], int bookCount) {
    FILE *file = fopen(BOOKS_FILE, "w");
    int index;

    if (file == NULL) {
        printf("Could not save books. Check that the data folder exists.\n");
        return;
    }

    for (index = 0; index < bookCount; index++) {
        fprintf(file, "%d|%s|%s|%d\n", books[index].id, books[index].title,
                books[index].author, books[index].quantity);
    }
    fclose(file);
}

static void add_book(Book books[], int *bookCount) {
    Book newBook;

    if (*bookCount >= MAX_BOOKS) {
        printf("The book list is full.\n");
        return;
    }

    newBook.id = read_non_negative_int("Book ID: ");
    if (find_book_by_id(books, *bookCount, newBook.id) != NULL) {
        printf("That book ID already exists.\n");
        return;
    }

    read_line("Book title: ", newBook.title, TEXT_SIZE);
    read_line("Author name: ", newBook.author, TEXT_SIZE);
    newBook.quantity = read_non_negative_int("Quantity: ");
    books[*bookCount] = newBook;
    (*bookCount)++;
    printf("Book added successfully.\n");
}

static void update_book(Book books[], int bookCount) {
    int choice;
    int id;
    char searchText[TEXT_SIZE];
    Book *book = NULL;
    int index;

    choice = read_int("Update by 1) ID  2) Name  3) Back: ");
    if (choice == 1) {
        id = read_int("Book ID: ");
        book = find_book_by_id(books, bookCount, id);
    } else if (choice == 2) {
        read_line("Book name to find: ", searchText, TEXT_SIZE);
        for (index = 0; index < bookCount; index++) {
            if (strstr(books[index].title, searchText) != NULL) {
                book = &books[index];
                break;
            }
        }
    } else {
        return;
    }

    if (book == NULL) {
        printf("Book not found.\n");
        return;
    }

    read_line("New title: ", book->title, TEXT_SIZE);
    read_line("New author: ", book->author, TEXT_SIZE);
    book->quantity = read_non_negative_int("New quantity: ");
    printf("Book updated successfully.\n");
}

static void remove_book(Book books[], int *bookCount) {
    int choice;
    int id;
    char searchText[TEXT_SIZE];
    int foundIndex = -1;
    int index;

    choice = read_int("Remove by 1) ID  2) Name  3) Back: ");
    if (choice == 1) {
        id = read_int("Book ID: ");
        for (index = 0; index < *bookCount; index++) {
            if (books[index].id == id) {
                foundIndex = index;
                break;
            }
        }
    } else if (choice == 2) {
        read_line("Book name to find: ", searchText, TEXT_SIZE);
        for (index = 0; index < *bookCount; index++) {
            if (strstr(books[index].title, searchText) != NULL) {
                foundIndex = index;
                break;
            }
        }
    } else {
        return;
    }

    if (foundIndex == -1) {
        printf("Book not found.\n");
        return;
    }

    for (index = foundIndex; index < *bookCount - 1; index++) {
        books[index] = books[index + 1];
    }
    (*bookCount)--;
    printf("Book removed successfully.\n");
}

static void search_books(const Book books[], int bookCount) {
    int choice;
    int id;
    char searchText[TEXT_SIZE];
    int index;
    int found = 0;

    choice = read_int("Search by 1) ID  2) Name  3) Author  4) Back: ");
    if (choice == 1) {
        id = read_int("Book ID: ");
        for (index = 0; index < bookCount; index++) {
            if (books[index].id == id) {
                print_book(&books[index]);
                found = 1;
            }
        }
    } else if (choice == 2 || choice == 3) {
        read_line(choice == 2 ? "Book name: " : "Author name: ",
                   searchText, TEXT_SIZE);
        for (index = 0; index < bookCount; index++) {
            if ((choice == 2 && strstr(books[index].title, searchText) != NULL) ||
                (choice == 3 && strstr(books[index].author, searchText) != NULL)) {
                print_book(&books[index]);
                found = 1;
            }
        }
    }

    if (choice >= 1 && choice <= 3 && !found) {
        printf("No matching books found.\n");
    }
}

static void show_all_books(const Book books[], int bookCount) {
    int index;

    if (bookCount == 0) {
        printf("No books are currently stored.\n");
        return;
    }

    printf("\n%-6s %-30s %-25s %s\n", "ID", "Title", "Author", "Quantity");
    printf("--------------------------------------------------------------------------\n");
    for (index = 0; index < bookCount; index++) {
        print_book(&books[index]);
    }
}

void book_menu(Book books[], int *bookCount) {
    int choice;

    do {
        printf("\n--- Book Management ---\n");
        printf("1. Add new book\n2. Update book\n3. Remove book\n");
        printf("4. Search book\n5. View all books\n6. Back\n");
        choice = read_int("Choose an option: ");

        switch (choice) {
            case 1: add_book(books, bookCount); break;
            case 2: update_book(books, *bookCount); break;
            case 3: remove_book(books, bookCount); break;
            case 4: search_books(books, *bookCount); break;
            case 5: show_all_books(books, *bookCount); break;
            case 6: break;
            default: printf("Please choose an option from 1 to 6.\n");
        }
    } while (choice != 6);
}
