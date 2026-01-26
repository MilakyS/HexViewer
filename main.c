#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET "\033[0m"


const char* GREEN = "\033[32m";
const char* RED = "\033[31m";
const char* BOLD = "\033[1m";



FILE* parse_args(int argc, char** argv) {
    FILE* file;

    if (argc < 2 || argc > 3) {
        printf("%s%sError: File required%s\n", BOLD, RED, RESET);
        return nullptr;
    }

    if (argc == 2) {
        file = fopen(argv[1], "rb");
    }
    else {
        file = fopen(argv[2], "rb");
    }
    return file;
}


void set_colors(int argc, char *argv[]) {
    if (argc == 3 && strcmp(argv[1], "-wcol") == 0) {
        GREEN = "";
        RED = "";
        BOLD = "";
    }
}


int main(int argc, char *argv[]) {
    set_colors(argc, argv);

    unsigned char buffer[16];
    int offset = 0;
    size_t counter;

    FILE *file = parse_args(argc, argv);
    if (!file) {
        fprintf(stderr, "%s%sError opening file%s\n", BOLD, RED, RESET);
        return EXIT_FAILURE;
    }

    while ((counter = fread(buffer, 1, 16, file)) != 0) {

        printf("%08X " , offset);
        for (int i = 0; i < 16; i++) {
            if (i < (int) counter) {
                printf("%s%02X %s", GREEN, buffer[i], RESET);
            }
            else {
                printf("   ");
            }
            if (7 == i) printf(" ");

        }

        printf(" | ");
        for (int i = 0; i < (int) counter; i++) {
            if (buffer[i] >= 32 && buffer[i] <= 126) {
                printf("%c", buffer[i]);
            }
            else {
                printf(".");
            }
        }
        printf(" |\n");

        offset += counter;
    }
    fclose(file);

    return EXIT_SUCCESS;
}
