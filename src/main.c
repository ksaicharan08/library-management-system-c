#include <stdio.h>

#include "library.h"

static void print_main_menu(void) {
    printf("\n========================================\n");
    printf("       LIBRARY MANAGEMENT SYSTEM\n");
    printf("========================================\n");
    printf("1. Book management\n");
    printf("2. Issue book\n");
    printf("3. Return book\n");
    printf("4. List issue records\n");
    printf("5. Save data\n");
    printf("6. Exit\n");
}

int main(void) {
    Book books[MAX_BOOKS];
    IssueRecord issues[MAX_ISSUES];
    int bookCount;
    int issueCount;
    int choice;

    /* Load old data first, so work is not lost when the program restarts. */
    load_books(books, &bookCount);
    load_issues(issues, &issueCount);

    printf("Welcome to the Library Management System.\n");
    do {
        print_main_menu();
        choice = read_int("Choose an option: ");

        switch (choice) {
            case 1:
                book_menu(books, &bookCount);
                break;
            case 2:
                issue_book(books, bookCount, issues, &issueCount);
                break;
            case 3:
                return_book(books, bookCount, issues, issueCount);
                break;
            case 4:
                list_issued_books(books, bookCount, issues, issueCount);
                break;
            case 5:
                save_books(books, bookCount);
                save_issues(issues, issueCount);
                printf("All data saved.\n");
                break;
            case 6:
                /* Saving during exit protects the latest changes. */
                save_books(books, bookCount);
                save_issues(issues, issueCount);
                printf("Data saved. Goodbye.\n");
                break;
            default:
                printf("Please choose an option from 1 to 6.\n");
        }
    } while (choice != 6);

    return 0;
}
