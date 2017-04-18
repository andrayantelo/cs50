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
    
    float factor = atof(argv[1]);
    //sscanf(argv[1], "%f", &factor);
    printf("factor: %f\n", factor);
    //factor = atoi(argv[1]);
    
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
    printf("number of pixels in original image: %d\n", og_biWidth);
    LONG og_biHeight = bi.biHeight;
    
    // Update outfile's header info
    bi.biWidth = bi.biWidth *= factor;
    printf("number of pixels in new image: %d\n", bi.biWidth);
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
    printf("scanlineWidth: %d\n", scanlineWidth);
    
    // TEMPORARY storage unit
    // use malloc here somehow.
    
    // round factor up
    float up_factor = ceil(factor);
    
    double memorySize = sizeof(RGBTRIPLE)*up_factor; //in bytes, if factor = 1.5, then memorySize = 6 bytes
                                               // R-1 byte, G-1 byte, B-1 Byte (space for TWO pixels)
                                               
    //round factor down
    float down_factor = floor(factor);
    
    double lastMemory = sizeof(RGBTRIPLE)*down_factor;
    
    printf("size of memory allocated: %f bytes\n", memorySize);
    RGBTRIPLE* temp = malloc(memorySize);
    
    RGBTRIPLE* tempFinal = malloc(lastMemory);
    printf("size of RGPTRIPLE: %zu bytes\n", sizeOfTriple);
    
    // keep track of which scanline you are on
    int row_tracker = 0;
    // keep track of where we are on the scanline
    int pixel_tracker = 0;
    
    int i;
    int r;
    int p;
    int j;
    int k;
    int end;
    int biHeight = abs(og_biHeight); // TODO is this the right height?
    
    // iterate over scanlines (rows)
    for (i = 0; i < biHeight; i++)
    {
        // repeat row factor times
        if (row_tracker % up_factor == 0) {
            end = up_factor;
        }
        else {
            end = down_factor;
        }
        for (r = 0; r <end; r++) {
            
            // iterate over pixels in scanline
            for (j = 0; j < og_biWidth; j++)
            {
                
                    // temporary storage
                    RGBTRIPLE triple;
        
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    
                    // repeat pixel factor times
                    for (p = 0; p < end; p++) {
                        // if tracker is one before end of scanline
                        if (tracker == bi.biWidth) {
                            // reset tracker
                            tracker = 0;
                            temp[p] = triple;
                            break;
                        }
                        temp[p] = triple;
                        tracker += 1;
                    }
                    
                    // write RGB triple to outfile 
                    fwrite(temp, sizeof(RGBTRIPLE), factor, outptr);
                    pixel_tracker += 1;
                
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
            
            row_tracker += 1;
            
        } // end of repeat row for loop
        
        // Move on to next row
        fseek(inptr, scanlineWidth, SEEK_CUR);

    }// end of row
    
    // free mallocs
    free(temp);
    free(tempFinal);
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    
    // success
    return 0;
}
