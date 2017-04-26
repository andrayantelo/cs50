#include <stdio.h>

int main(int argc, char *argv[]) {
    
    // Ensure only one command line argument
    if (argc != 1) {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }
    
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
}
