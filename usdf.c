#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LEN 256

void extract_variables(const char *text, const char *label) {
    printf("  %s: ", label);
    int found = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '%' && text[i + 1] == 'v') {
            int j = i + 2;
            if (isdigit(text[j])) {
                printf("%%v");
                while (isdigit(text[j])) {
                    putchar(text[j]);
                    j++;
                }
                putchar(' ');
                found = 1;
                i = j - 1; 
            }
        }
    }

    if (!found) printf("(none)");
    printf("\n");
}

void main() {
    FILE *file = fopen("variablelang", "r");
    if (!file) {
        printf("Unable to open file");
        exit(1);
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
        char *arrow = strstr(line, "<-");
        if (arrow) {
            *arrow = '\0';
            char *left = line;
            char *right = arrow + 2;

            printf("Instruction: %s <- %s", left, right);

            right[strcspn(right, "\n")] = 0;

            extract_variables(left, "Defined");
            extract_variables(right, "Used");
            printf("\n");
        }
    }

    fclose(file);
}

