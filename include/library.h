#ifndef LIBRARY_H
#define LIBRARY_H

#include <time.h>

#define MAX_BOOKS 100
#define MAX_ISSUES 200
#define TEXT_SIZE 100
#define DATE_SIZE 11
#define FINE_PER_DAY 5

/* A book stored in the library. */
typedef struct {
    int id;
    char title[TEXT_SIZE];
    char author[TEXT_SIZE];
    int quantity;
} Book;

/* One borrowing record. It remains in the file after a book is returned. */
typedef struct {
    int issueId;
    int bookId;
    int userId;
    char userName[TEXT_SIZE];
    char issueDate[DATE_SIZE];
    char dueDate[DATE_SIZE];
    char returnDate[DATE_SIZE];
    int fineAmount;
    int returned;
} IssueRecord;

/* Input helpers. */
void read_line(const char *message, char *value, int size);
int read_int(const char *message);
int read_non_negative_int(const char *message);

/* Date helpers. */
void get_today(char *date);
void get_due_date(const char *issueDate, char *dueDate, int daysToAdd);
int days_between(const char *firstDate, const char *secondDate);

/* Book module. */
void load_books(Book books[], int *bookCount);
void save_books(const Book books[], int bookCount);
void book_menu(Book books[], int *bookCount);
Book *find_book_by_id(Book books[], int bookCount, int id);

/* Issue and return module. */
void load_issues(IssueRecord issues[], int *issueCount);
void save_issues(const IssueRecord issues[], int issueCount);
void issue_book(Book books[], int bookCount, IssueRecord issues[], int *issueCount);
void return_book(Book books[], int bookCount, IssueRecord issues[], int issueCount);
void list_issued_books(const Book books[], int bookCount,
                       const IssueRecord issues[], int issueCount);

#endif
