#include <stdio.h>

int main(int argc, char *argv[]) {
    
    // Ensure only one command line argument
    if (argc != 2) {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // Remember file names
    char *infile = argv[1];
    
    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    printf("size of image file: %zu\n", sizeof(infile));
}
