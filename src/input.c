#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.h"

void read_line(const char *message, char *value, int size) {
    int length;

    printf("%s", message);
    if (fgets(value, size, stdin) == NULL) {
        value[0] = '\0';
        return;
    }

    length = (int)strlen(value);
    if (length > 0 && value[length - 1] == '\n') {
        value[length - 1] = '\0';
    } else {
        /* Remove extra characters if the user typed more than the buffer. */
        int character;
        while ((character = getchar()) != '\n' && character != EOF) {
            /* Keep reading until the input line is empty. */
        }
    }
}

int read_int(const char *message) {
    char text[TEXT_SIZE];
    char extra;
    int value;

    while (1) {
        read_line(message, text, sizeof(text));
        if (sscanf(text, " %d %c", &value, &extra) == 1) {
            return value;
        }
        printf("Please enter a whole number.\n");
    }
}

int read_non_negative_int(const char *message) {
    int value;

    do {
        value = read_int(message);
        if (value < 0) {
            printf("The number cannot be negative.\n");
        }
    } while (value < 0);

    return value;
}
