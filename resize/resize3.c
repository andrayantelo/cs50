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
    bi.biWidth *= factor;
    bi.biHeight *= factor;
    
    printf("new biWidth: %d pixels\n", bi.biWidth);
    printf("new biHeight: %d pixels\n", bi.biHeight);

    // determine padding for scanlines
    int og_padding = (4 - (og_biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    printf("old image padding: %d bytes\n", og_padding);
    printf("new image padding: %d bytes\n", padding);
    
    // store old image size
    int og_biSizeImage = bi.biSizeImage;
    printf("size of old image: %d bytes\n",og_biSizeImage);

    bi.biSizeImage = ((sizeof(RGBTRIPLE)*bi.biWidth) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    printf("Size of new image: %d bytes\n", bi.biSizeImage);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // I use this later when calculating row widths
    size_t sizeOfTriple = sizeof(RGBTRIPLE);
    
    // each pixel is 3 bytes, scanlineWidth is in bytes. (36 bytes -> 12 pixels);
    // OG scanlineWidth
    int oldRowWidth = og_biWidth*sizeOfTriple + og_padding;
    
    // allocate memory to temporarily store row of old image
    BYTE* old = calloc(og_biSizeImage, 1);
    
    // new image's row width including padding in BYTES
    int newRowWidth = bi.biWidth*sizeOfTriple + padding; // in bytes

    //allocate memory to temporarily store new image
    BYTE* new = calloc(bi.biSizeImage, 1);

    // read image from infile and temporarily store its bytes in old
    fread(old, 1, og_biSizeImage, inptr);
    
    int i;
    int j;
   
    int biHeight = abs(bi.biHeight); //#number of rows
    
    // iterate over new scanlines (rows)
    for (i = 0; i < biHeight; i++) {
        
        // determine the row from old to be copied
        RGBTRIPLE* old_pixel =  (RGBTRIPLE*) (old + (int) floor(i/factor));
    
        //set new_pixel to the address of the beginning of current row, padding included in newRowWidth
        RGBTRIPLE* new_pixel = (RGBTRIPLE*) (new + i*newRowWidth);
        
            // iterate over new image pixels in scanline (row)
            for (j = 0; j < bi.biWidth; j++) {   
                new_pixel[j] = old_pixel[(int) floor(j/factor)];
                //printf("%4i %4i\n", i, j);
         
            }

            
    }// end of repeat row for loop
    
    fwrite(new, 1, bi.biSizeImage, outptr);

    // free mallocs
    free(old);
    free(new);
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    
    // success
    return 0;
}
