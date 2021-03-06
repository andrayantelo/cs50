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
    
    int factor;
    //sscanf(argv[1], "%f", &factor);
    factor = atoi(argv[1]);
    
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
    
    // Update outfile's header info
    bi.biWidth = bi.biWidth *= factor;
    bi.biHeight = bi.biHeight *= factor;

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
    // each pixel is 3 bytes, scanlineWidth is in bytes. (36 bytes -> 12 pixels);
    int scanlineWidth = og_biWidth*sizeOfTriple + og_padding;
    
    // TEMPORARY storage unit
    int memorySize = sizeof(RGBTRIPLE)*factor; //in bytes, if factor = 2, then memorySize = 6 bytes
                                               // R-1 byte, G-1 byte, B-1 Byte (space for TWO pixels)
    
    RGBTRIPLE* temp = malloc(memorySize);
    
    int i;
    int r;
    int p;
    int j;
    int k;
    int biHeight = abs(og_biHeight); // TODO is this the right height?
    
    // iterate over scanlines (rows)
    for (i = 0; i < biHeight; i++)
    {
        // repeat row factor times
        for (r = 0; r < factor; r++) {
            
            // iterate over pixels in scanline
            for (j = 0; j < og_biWidth; j++)
            {   
                
                    // temporary storage
                    RGBTRIPLE triple;
        
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    
                    // repeat pixel factor times
                    for (p = 0; p < factor; p++) {
                        temp[p] = triple;
                    }
                    
                    // write RGB triple to outfile 
                    fwrite(temp, sizeof(RGBTRIPLE), factor, outptr);
                
            }
            
            // skip over padding, if any
            fseek(inptr, og_padding, SEEK_CUR);
    
            // then add it back (to demonstrate how)
            for (k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
            // done with one row
            // go back to beginning of row
            fseek(inptr, -scanlineWidth, SEEK_CUR);
            
        } // end of repeat row for loop
        
        // Move on to next row
        fseek(inptr, scanlineWidth, SEEK_CUR);

    }// end of row
    
    // free mallocs
    free(temp);
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    
    // success
    return 0;
}
