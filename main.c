#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct {
    const char *RESET;
    const char *GREEN;
    const char *RED;
    const char *BOLD;
}Color;

Color color;




FILE* parse_args(const int argc, char** argv) {
    FILE* file;

    if (argc < 2 || argc > 3) {
        printf("%s%sError: File required%s\n", color.BOLD, color.RED, color.RESET);
        return NULL;
    }

    if (argc == 2) {
        file = fopen(argv[1], "rb");
    }
    else {
        file = fopen(argv[2], "rb");
    }
    return file;
}


void set_colors(const int argc, char *argv[]) {
    color.RESET = "\033[0m";
    color.GREEN = "\033[32m";
    color.RED = "\033[31m";
    color.BOLD = "\033[1m";
    //TODO add support for flags in any position
    if (argc == 3 && strcmp(argv[1], "-wcol") == 0 || !isatty(STDOUT_FILENO)) {
        color.RESET = color.GREEN = color.RED = color.BOLD = "";
    }
}


int main(const int argc, char *argv[]) {
    set_colors(argc, argv);

    unsigned char buffer[16];
    int offset = 0;
    size_t counter;

    FILE *file = parse_args(argc, argv);
    if (!file) {
        fprintf(stderr, "%s%sError opening file: %s %s\n",
            color.BOLD, color.RED, strerror(errno), color.RESET);
        return EXIT_FAILURE;
    }

    while ((counter = fread(buffer, 1, 16, file)) != 0) {

        printf("%08X " , offset);
        for (int i = 0; i < 16; i++) {
            if (i < (int) counter) {
                printf("%s%02X %s", color.GREEN, buffer[i], color.RESET);
            }
            else {
                printf("   ");
            }
            if (7 == i) printf(" ");

        }

        printf(" | ");
        //FIXME
        for (int i = 0; i < (int) counter; i++) {
            if (buffer[i] >= 32 && buffer[i] <= 126) {
                printf("%c", buffer[i]);
            }
            else {
                printf(".");
            }
        }
        printf(" |\n");

        offset += (int) counter;
    }
    fclose(file);

    return EXIT_SUCCESS;
}
