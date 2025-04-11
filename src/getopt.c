#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/getopt.h"

bool init = true;
int i_argc = 0;
int i_flag = 0;
int i_farg = 0;
char **fargv; // argumenty flagi
char **vargv; // wektor argumentów bez flag
int fargc; // ilość argumentów w fargv
int vargc; // ilość argumentów w vargv
char cflag ; // obecna flaga
size_t size_fargv = DEFAULT_VECTOR_SIZE;
size_t size_vargv = DEFAULT_VECTOR_SIZE;

int *argq;

void double_size(char ***vector, size_t *current) {
    *current *= 2;

    void *new_ptr = realloc(*vector, *current);
    if (!new_ptr) {
        fprintf(stderr, "Getopt error: allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    *vector = new_ptr;
    for (size_t i = *current/2; i < *current; i++)
        *(vector + i) = NULL;
}

bool init_getopt(const char *format) {
    init = false;

    fargv = calloc(DEFAULT_VECTOR_SIZE, sizeof(char*));
    vargv = calloc(DEFAULT_VECTOR_SIZE, sizeof(char*));
    fargc = 0;
    vargc = 0;

    argq = malloc(26 * sizeof(int));
    for (int i = 0; i < 26; i++)
        *(argq + i) = -1;

    if (*format == ':') {
        fprintf(stderr, "Getopt error: first argument without a flag: '%s'.\n", format);
        return false;
    }

    const char *c = format;
    const char *k = c;
    const char *f = c;
    while (*c) {
        if (*c == ':') {
            (*(argq + *f - 'a'))++;
            c++;
            continue;
        }

        if (*c < 'a' || *c > 'z') {
            fprintf(stderr, "Getopt error: wrong flag character: '%c'. Only lowercase alphabet letters are allowed.\n", *c);
            return false;
        }

        k = c;
        while (*k++) {
            if (*k != ':' && *c == *k) {
                fprintf(stderr, "Getopt error: duplicate flag: '%c'.\n", *c);
                return false;
            }
        }

        f = c;
        (*(argq + *c - 'a'))++;
        c++;
    }
    return true;
}

char ogetopt(int argc, char **argv, const char *format) {
    if ((init && !init_getopt(format)) || i_argc == argc) {
        return -1;
    }

    for (int i = 0; i < fargc; i++)
        fargv[i] = NULL;
    fargc = 0;

    if (**(argv + i_argc) != '-') {
        if (vargc == size_vargv - 1)
            double_size(&vargv, &size_vargv);

        *(vargv + vargc) = *(argv + i_argc);
        vargc++;
        i_argc++;
        return '.';
    }

    cflag = *(*(argv + i_argc) + i_flag + 1);
    if (!cflag) {
        i_argc+=i_farg + 1;
        i_farg = 0;
        i_flag = 0;
        return ogetopt(argc, argv, format);
    }

    if (cflag < 'a' || cflag > 'z' || *(argq + cflag -'a') == -1) {
        i_flag++;
        return '?';
    }

    if (!*(argq + cflag - 'a')) {
        i_flag++;
        return cflag;
    }

    fargc = *(argq + cflag - 'a');
    for (int i = 0; i < *(argq + cflag - 'a'); i++, i_farg++) {
        if (fargc == size_fargv - 1)
            double_size(&fargv, &size_fargv);

        if (i_argc + i_farg + 1 >= argc || **(argv + i_argc + i_farg + 1) == '-') {
            fargc = i;
            i_flag++;
            return ':';
        }
        *(fargv + i) = *(argv + i_argc + i_farg + 1);
    }
    i_flag++;
    return cflag;
}
