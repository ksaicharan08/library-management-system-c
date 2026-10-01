#include <stdio.h>
#include <string.h>

#include "library.h"

#define ISSUES_FILE "data/issues.txt"
#define LOAN_DAYS 7

static IssueRecord *find_open_issue(IssueRecord issues[], int issueCount,
                                    int bookId, int userId) {
    int index;

    for (index = 0; index < issueCount; index++) {
        if (!issues[index].returned && issues[index].bookId == bookId &&
            issues[index].userId == userId) {
            return &issues[index];
        }
    }
    return NULL;
}

void load_issues(IssueRecord issues[], int *issueCount) {
    FILE *file = fopen(ISSUES_FILE, "r");
    char line[TEXT_SIZE * 4];
    IssueRecord issue;

    *issueCount = 0;
    if (file == NULL) {
        return;
    }

    while (*issueCount < MAX_ISSUES && fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d|%d|%d|%99[^|]|%10[^|]|%10[^|]|%10[^|]|%d|%d",
                   &issue.issueId, &issue.bookId, &issue.userId,
                   issue.userName, issue.issueDate, issue.dueDate,
                   issue.returnDate, &issue.fineAmount, &issue.returned) == 9) {
            issues[*issueCount] = issue;
            (*issueCount)++;
        }
    }
    fclose(file);
}

void save_issues(const IssueRecord issues[], int issueCount) {
    FILE *file = fopen(ISSUES_FILE, "w");
    int index;

    if (file == NULL) {
        printf("Could not save issue records. Check that the data folder exists.\n");
        return;
    }

    for (index = 0; index < issueCount; index++) {
        fprintf(file, "%d|%d|%d|%s|%s|%s|%s|%d|%d\n",
                issues[index].issueId, issues[index].bookId, issues[index].userId,
                issues[index].userName, issues[index].issueDate,
                issues[index].dueDate, issues[index].returnDate,
                issues[index].fineAmount, issues[index].returned);
    }
    fclose(file);
}

void issue_book(Book books[], int bookCount, IssueRecord issues[], int *issueCount) {
    int bookId;
    int userId;
    Book *book;
    IssueRecord newIssue;
    int lastIssueId = 0;
    int index;

    if (*issueCount >= MAX_ISSUES) {
        printf("The issue list is full.\n");
        return;
    }

    bookId = read_int("Book ID to issue: ");
    book = find_book_by_id(books, bookCount, bookId);
    if (book == NULL) {
        printf("Book not found.\n");
        return;
    }
    if (book->quantity == 0) {
        printf("This book is not available right now.\n");
        return;
    }

    userId = read_int("User ID: ");
    if (find_open_issue(issues, *issueCount, bookId, userId) != NULL) {
        printf("This user already has this book.\n");
        return;
    }

    for (index = 0; index < *issueCount; index++) {
        if (issues[index].issueId > lastIssueId) {
            lastIssueId = issues[index].issueId;
        }
    }

    newIssue.issueId = lastIssueId + 1;
    newIssue.bookId = bookId;
    newIssue.userId = userId;
    read_line("User name: ", newIssue.userName, TEXT_SIZE);
    get_today(newIssue.issueDate);
    get_due_date(newIssue.issueDate, newIssue.dueDate, LOAN_DAYS);
    strcpy(newIssue.returnDate, "-");
    newIssue.fineAmount = 0;
    newIssue.returned = 0;

    issues[*issueCount] = newIssue;
    (*issueCount)++;
    book->quantity--;

    printf("Book issued successfully.\n");
    printf("Issue date: %s | Due date: %s\n", newIssue.issueDate, newIssue.dueDate);
}

void return_book(Book books[], int bookCount, IssueRecord issues[], int issueCount) {
    int bookId;
    int userId;
    Book *book;
    IssueRecord *issue;
    int lateDays;

    bookId = read_int("Book ID to return: ");
    userId = read_int("User ID: ");
    issue = find_open_issue(issues, issueCount, bookId, userId);
    if (issue == NULL) {
        printf("Open issue record not found. Check the book and user IDs.\n");
        return;
    }

    get_today(issue->returnDate);
    lateDays = days_between(issue->dueDate, issue->returnDate);
    if (lateDays < 0) {
        lateDays = 0;
    }
    issue->fineAmount = lateDays * FINE_PER_DAY;
    issue->returned = 1;

    book = find_book_by_id(books, bookCount, bookId);
    if (book != NULL) {
        book->quantity++;
    }

    printf("Book returned successfully on %s.\n", issue->returnDate);
    printf("Late days: %d | Fine: Rs. %d\n", lateDays, issue->fineAmount);
}

void list_issued_books(const Book books[], int bookCount,
                       const IssueRecord issues[], int issueCount) {
    int index;
    int bookIndex;
    int showRecord;
    const Book *book;

    if (issueCount == 0) {
        printf("No issue records are currently stored.\n");
        return;
    }

    printf("\nIssue records:\n");
    for (index = 0; index < issueCount; index++) {
        book = NULL;
        for (bookIndex = 0; bookIndex < bookCount; bookIndex++) {
            if (books[bookIndex].id == issues[index].bookId) {
                book = &books[bookIndex];
                break;
            }
        }
        showRecord = issues[index].returned == 0;
        printf("\nIssue ID: %d | Status: %s\n", issues[index].issueId,
               showRecord ? "Issued" : "Returned");
        printf("Book: %s (ID %d)\n", book == NULL ? "Removed book" : book->title,
               issues[index].bookId);
        printf("User: %s (ID %d)\n", issues[index].userName, issues[index].userId);
        printf("Issue date: %s | Due date: %s | Return date: %s\n",
               issues[index].issueDate, issues[index].dueDate,
               issues[index].returnDate);
        printf("Fine: Rs. %d\n", issues[index].fineAmount);
    }
}
