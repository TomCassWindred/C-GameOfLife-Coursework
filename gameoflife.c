#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include"gol.h"



int main(int argc, char *argv[]) {
    //Initialise needed variables
    struct universe v;

    FILE *inpfile;
    FILE *outfile;

    bool (*will_be_alive_func)(struct universe *u, int column, int row) = &will_be_alive;
    bool printstats = false;
    int gencount = 5;

    char *inputfilename = NULL;
    char *outputfilename = NULL;

    //Parse the input arguments
    int opt;
    while ((opt = getopt(argc, argv, ":i:o:g:st")) != -1) {
        switch (opt) {
            case '?':
                printf("-%c is not a recognised option.\n", optopt);
                break;
            case ':':
                printf("Missing argument for option -%c, ignoring option.\n", opt);
                break;
            case 'i':
                if (optarg[0] == '-') {
                    fprintf(stderr,"Missing valid argument for option -i, please try again with valid argument...\n");
                    exit(7);
                }
                printf("Setting input file to %s...\n", optarg);
                inputfilename = optarg;
                break;
            case 'o':
                if (optarg[0] == '-') {
                    fprintf(stderr, "Missing valid argument for option -o, please try again with valid argument...\n");
                    exit(8);
                }
                //printf("Setting output file to %s...\n", optarg);
                outputfilename = optarg;
                break;
            case 'g':
                if (optarg[0] == '-') {
                    fprintf(stderr, "Missing valid argument for option -g, please try again with valid argument...\n");
                    exit(9);
                }
                //printf("Setting generation count to %s\n", optarg);
                gencount = strtol(optarg, NULL, 10);
                break;
            case 's':
                //printf("Printing statistics at end of run.\n");
                printstats = true;
                break;
            case 't':
                //printf("Using Torus configuration\n");
                will_be_alive_func = &will_be_alive_torus;
                break;
        }
    }


    if (outputfilename == NULL) {
        //printf("No output file specified, using STDOUT, the result will be printed below\n");
        outfile = stdout;
    } else if ((outfile = fopen(outputfilename, "w")) == NULL) { //Opens file and returns NULL if failed
        perror("Error opening output file, please ensure the file you entered exists in same directory.\n");
        // Program exits if file pointer returns NULL.
        exit(5);
    }

    if (inputfilename == NULL) {
        //printf("No input file specified, using STDIN, type your input below and use Ctrl+D when done\n");
        inpfile = stdin;
    } else if ((inpfile = fopen(inputfilename, "r")) == NULL) { //Opens file and returns NULL if failed
        perror("Error opening input file, please ensure the file you entered exists in same directory.\n");
        // Program exits if file pointer returns NULL.
        exit(6);
    }


    read_in_file(inpfile, &v);

    //printf("Input successful, input universe is as follows:\n");
    //write_out_file(stdout, &v);


    for (int i = 0; i < gencount; ++i) {
        evolve(&v, will_be_alive_func);

    }

    //printf("Simulation Successful, final universe is as follows:\n");
    write_out_file(outfile, &v);

    if (printstats == true) {
        print_statistics(&v);
    }

    return 0;
}
