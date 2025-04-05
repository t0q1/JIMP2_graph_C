#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv)
{


    FILE *in = argc > 1 ? fopen(argv[1], "r") : fprintf(stderr, "Zbyt mala liczba argumentow. Nalezy podac sciezke pliku wejsciowego!");

    if (in == NULL)
    {
        fprintf(stderr, "Nie udalo sie otworzyc pliku wejsciowego o podanej sciezkce. Przerywam dzialanie.");
        return 1;
    }

    int N = argc > 2 ? atoi(argv[2]) : 1;
    int M = argc > 3 ? atoi(argv[3]) : 10;


    int opt;
    int binary_output = 0;
    int terminal_output = 0;
    int file_output = 0;
    char * output_filename = "wynik.txt";
    while ((opt = getopt(argc, argv, "bto:")) != -1)
    {
        switch (opt) {
            case 'b':
                binary_output = 1;
                break;
            case 't':
                terminal_output = 1;
                break;
            case 'o':
                file_output = 1;
                char * output_filename = optarg;
                break;
        }
    }
    if (terminal_output == 0) file_output = 1; // jesli flaga t bez o, to tylko terminal. Jesli brak jakiejkolwiek flagi to wyjscie to pliku


    

    return 0;
}