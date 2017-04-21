// allocate memory to temporarily store old image BYTES and do the same
// for new image

BYTE* old = calloc(size of old img in bytes);
BYTE* new = calloc(size of new img in bytes);

// make pointer of type RGBTRIPLE and point to same address as old and make one
// for new also, so that you can move sizeof(RGBTRIPLE) through the array (done inside for loop)

//build new image

//loop through new image rows
for (i = 0; i < rows_in_new_image; i++) { 
    
    // determine the row from old to be copied
    RGBTRIPLE* old_pixel =  (RGBTRIPLE*) (old + (int) floor(i/factor));
    
    //set new_pixel to the address of the beginning of current row, padding included in newRowWidth
    RGBTRIPLE* new_pixel = (RGBTRIPLE*) (new + i*newRowWidth);
    
    //for each 'new' pixel i in row
    for (j=0; j < pixels_in_new_row; j++) {
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
