#include <stdio.h>
#include <time.h>

#include "library.h"

static time_t date_to_time(const char *date) {
    struct tm parsedDate = {0};
    int year;
    int month;
    int day;

    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) {
        return (time_t)-1;
    }

    parsedDate.tm_year = year - 1900;
    parsedDate.tm_mon = month - 1;
    parsedDate.tm_mday = day;
    parsedDate.tm_hour = 12;
    return mktime(&parsedDate);
}

void get_today(char *date) {
    time_t now = time(NULL);
    struct tm *today = localtime(&now);

    strftime(date, DATE_SIZE, "%Y-%m-%d", today);
}

void get_due_date(const char *issueDate, char *dueDate, int daysToAdd) {
    time_t date = date_to_time(issueDate);
    struct tm due;

    if (date == (time_t)-1) {
        dueDate[0] = '\0';
        return;
    }

    due = *localtime(&date);
    due.tm_mday += daysToAdd;
    mktime(&due);
    strftime(dueDate, DATE_SIZE, "%Y-%m-%d", &due);
}

int days_between(const char *firstDate, const char *secondDate) {
    time_t first = date_to_time(firstDate);
    time_t second = date_to_time(secondDate);
    double secondsPerDay = 60.0 * 60.0 * 24.0;

    if (first == (time_t)-1 || second == (time_t)-1) {
        return 0;
    }

    return (int)((second - first) / secondsPerDay);
}
