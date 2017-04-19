/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }
    
    float factor;
    //sscanf(argv[1], "%f", &factor);
    //factor = atoi(argv[1]);
    factor = atof(argv[1]);
    printf("factor : %f\n", factor);
    
    // factor must be between (0.0, 100.0]
    if (factor <= 0 || factor > 100) {
        fprintf(stderr, "Factor f must be between (0, 100]");
        return 1;
    }
  
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    
    // Remember old biWidth and biHeight
    LONG og_biWidth = bi.biWidth;
    LONG og_biHeight = bi.biHeight;
    
    printf("original bi_width: %d pixels\n", og_biWidth);
    printf("original bi_height: %d pixels\n", og_biHeight);
    
    // Update outfile's header info
    // float gets truncated
    bi.biWidth = bi.biWidth *= factor;
    bi.biHeight = bi.biHeight *= factor;
    
    printf("new biWidth: %d pixels\n", bi.biWidth);
    printf("new biHeight: %d pixels\n", bi.biHeight);

    // determine padding for scanlines
    int og_padding = (4 - (og_biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage = ((sizeof(RGBTRIPLE)*bi.biWidth) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    

    size_t sizeOfTriple = sizeof(RGBTRIPLE);
    printf("size of triple: %zu\n", sizeOfTriple);
    
    // each pixel is 3 bytes, scanlineWidth is in bytes. (36 bytes -> 12 pixels);
    // OG scanlineWidth
    int scanlineWidth = og_biWidth*sizeOfTriple + og_padding;
    int oldRowWidth = og_biWidth*sizeOfTriple;
    
    printf("size of malloc oldRow : %d in bytes\n", oldRowWidth);
    
    // allocate memory to temporarily store row of old image
    RGBTRIPLE* oldRow = malloc(oldRowWidth);
    
    // Temporary storage unit of size of new image's row
    int newRowWidth = bi.biWidth*sizeOfTriple + padding; // in bytes
    int newRowWidthWithoutPadding = bi.biWidth*sizeOfTriple;
    
    printf("size of malloc newRow: %d in bytes\n", newRowWidthWithoutPadding);
    
    //allocate memory to temporarily store row of new image
    RGBTRIPLE* newRow = malloc(newRowWidthWithoutPadding);
    
    printf("Old row width: %d bytes\n", scanlineWidth);
    printf("new row width: %d bytes\n", newRowWidth);
    
    int i;
    int r;

    int j;
    int k;
    
    int m;

    int biHeight = abs(og_biHeight); //pixels
    
    // iterate over OG scanlines (rows)
    for (i = 0; i < biHeight; i++)
    {
        // repeat row factor times
        for (r = 0; r < factor; r++) {
            
            // iterate over OG pixels in scanline (row)
            for (j = 0; j < og_biWidth; j++)
            {   
            // read from inptr and store the row in oldRow - temporary storage
            
                    // temporary storage
                    RGBTRIPLE triple;
        
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    
                    // store pixel in oldRow
                    oldRow[j] = triple;
            }

            // skip over padding, if any
            // this takes you to the beginning of the next row
            fseek(inptr, og_padding, SEEK_CUR);
            
            // after the preceding for loop, you have the original row stored
            // in 'oldRow'. Now you want to build the new row from the old one and write it into
            // the outfile
            
            //TODO BUILD ROW HERE
            for (m = 0; m < bi.biWidth; m++) {
                newRow[m] = oldRow[(int) floor(m/factor)];
            }
            
            // Now you have built the new row and stored it in 'newRow' temporarily.
            // You need to now write it into the outfile.
            
            fwrite(newRow, sizeof(newRowWidthWithoutPadding), 1, outptr);
            
            // add new padding of new row to outfile
            for (k = 0; k < padding; k++)
            {
                printf("adding one padding\n");
                fputc(0x00, outptr);
            }
            
            // Now you have written one new row into the outfile including its padding
            // We have to determine if we need to repeat this row, how many times
            // do we need to repeat it?

            fseek(inptr, -scanlineWidth, SEEK_CUR);
            
        } // end of repeat row for loop
        
        // Move on to next row
        fseek(inptr, scanlineWidth, SEEK_CUR);

    }// end of row
    
    // free mallocs
    free(oldRow);
    free(newRow);
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    
    // success
    return 0;
}
