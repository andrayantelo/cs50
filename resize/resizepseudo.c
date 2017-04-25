// allocate enough memory in BYTES for old image and set the allocated memory to zero
// allocate enough memory in BYTES for new image and set the allocated memory to zero

BYTE* old = calloc(og_biSizeImage, 1);
BYTE* new = calloc(bi.biSizeImage, 1);

// calculate new image's row width including padding in BYTES <-- TODO need this in pixels
int newRowWidth = bi.biWidth*sizeof(RGBTRIPLE) + padding;
    
// read the old image from infile and
// store it in the 'old' pointer
fread(old, 1, og_biSizeImage, inptr);

//build new image

//loop through new image rows (height)
int biHeight = abs(bi.biHeight); //#number of rows in new image

for (int i = 0; i < biHeight; i++) { 
    
    // make pointer of type RGBTRIPLE and point to same address as 'old'
    // make pointer of type RGBTRIPLE and point to same address as 'new'
    // this allows you to move sizeof(RGBTRIPLE) (pixel by pixel) through the array (when using new_pixel
    // or old_pixel (done inside for loop)
    
    // determine the row from old to be copied and assert it is not larger than 
    // the old image's height
    assert (i/factor) <  og_biHeight
    
    RGBTRIPLE* old_pixel =  (RGBTRIPLE*) (old + ((int) (i/factor)));
    
    //set new_pixel to the address of the beginning of current row, padding included in newRowWidth
    RGBTRIPLE* new_pixel = (RGBTRIPLE*) (new + i*newRowWidth);

    
    //for each 'new' pixel i in row
    for (int j = 0; j < bi.biWidth; j++) {
        
        // determine the pixel from 'old' to be copied and assert that its
        // index is not larger than og_biWidth
        assert j/factor < og_biWidth;
        
        new_pixel[j] = old_pixel[(int) floor(j/factor)];
    }
    // don't have to do next three lines  because using calloc
    //add padding, after pixel for loop, i + 1 points to the next pixel...
    //bzero(new_pixel[i+1], num_bytes_of_padding)
    // do that for the amount of padding

    //move on to next row in new... set new_pixel to the address of the next row, which is done on line 19
}



free(old)
free(new)
