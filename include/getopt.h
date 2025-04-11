#ifndef GETOPT_H
#define GETOPT_H

#define DEFAULT_VECTOR_SIZE 2

extern char **fargv; // argumenty flagi
extern char **vargv; // wektor argumentów bez flag
extern int fargc; // ilość argumentów w fargv
extern int vargc; // ilość argumentów w vargv

char ogetopt(const int argc, char **argv, const char *format);

#endif //GETOPT_H
