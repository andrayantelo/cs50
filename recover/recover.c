#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[]) {
    
    // Ensure only one command line argument
    if (argc != 2) {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // Remember file names
    char *infile = argv[1];
    
    // Open input file
    FILE *raw_file = fopen(infile, "r");
    if (raw_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    // Determine size of file
    fseek(raw_file, 0, SEEK_END);
    int fileLen = ftell(raw_file);
    fseek(raw_file, 0, SEEK_SET);
    
    printf("Size of image: %d bytes\n", fileLen);
    
    int numOfBlocks = fileLen/512;
    
    // How many blocks of 512 in the raw_file
    printf("NUmber of 512 blocks: %d\n", numOfBlocks);
    
    // Keep track of JPEGs found
    int jpegCounter = 0;
    
    // Jpeg file
    FILE *img = NULL;
    
    // Filename for JPEGs found
    char filename[8];
    
    // Temporary storage
    BYTE* buffer = malloc(512);

    // Loop over the 512 - blocks
    int j;
    
    for(j = 0; j < numOfBlocks; j++) {
        printf("j = %d\n", j);
        // Read jth 512 - block into buffer
        fread(buffer, 512, 1, raw_file);

        // Are we at the start of a new JPEG?
        if (buffer[0] == 0xff && buffer[1] == 0xd8 &&
           buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
            if (img == NULL) {
                if (jpegCounter == 0) {
                    printf("first jpeg\n");
                }
                else {
                    printf("not first jpeg\n");
                }
            }
            
            printf("Start of an image!\n");
            // Have we already found a JPEG?
            if (jpegCounter > 0) {
                // If we have, close the previous JPEG
                // so that we can start the new one
                fclose(img);
            }
            // if not, we're starting our very first JPEG file
            
            sprintf(filename, "%03i.jpg", jpegCounter);
            // Increment jpegCounter
            jpegCounter++;

            // Open a new JPEG
            img = fopen(filename, "w");
            
            printf("Writing first 512-block into jpeg\n");
            // Write 512-block into JPEG file
            fwrite(buffer, 512, 1, img);
        }
        
        // If we aren't at a new JPEG
        else {
            printf("Not start of an image\n");
            // if not currently in the middle of a JPEG
            // discard this 512-block
            if (img == NULL) {
                printf("jpeg not opened\n");
            }
            else {
                printf("jpeg file already opened\n");
                printf("On img: %s\n", filename);
                printf("writing 512-block into %s\n", filename);
                // write buffer into img file
                fwrite(buffer, 512, 1, img);
            }
        }
        
    }
    printf("Found %d images\n", jpegCounter);
    // Free mallocs
    free(buffer);
        
    
}
