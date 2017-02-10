#include <stdio.h>
//#include <cs50.h>
#include <stdbool.h>

void sort(int values[], int n);
bool search(int value, int values[], int n);

int main(void) {
    int values[3];
    values[0] = 40;
    values[1] = 41;
    values[2] = 42;


    
    //printf("values before sorting: \n");
    //int y;
    //for (y = 0; y < 5; y++) {
    //    printf("values: %d\n", values[y]);
    //}
    
    sort(values, 3);
    //printf("values after sorting: \n");
    //for (y = 0; y < 5; y++) {
    //    printf("values: %d\n", values[y]);
    //}
    
    bool found = search(42, values, 3);
    if (found == true) {
        printf("Found the value\n");
    }
    else {
        printf("Didn't find it: %d\n", found);
    }
}



void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    // Let's try selection sort.
   
    int i;
    int j;
    int place_counter = 0;
    
    for (i = 0; i < n; i++) {
        //Take ith element and compare it to all the other elements
        for (j = place_counter; j < n; j++) {
            if (values[i] > values[j]) {
                //break out of loop
                j = n;
            }
            
        }
        if (j == n) {
            // Swap places between the smallest element and the element currently in the first position
            int smallest_element = values[i];
            int swap_element = values[place_counter];

            values[i] = swap_element;
            values[place_counter] = smallest_element;
            
            // have to move to the next value in values.. the value after the recently sorted smallest element.
            i = place_counter;
            
            place_counter += 1;
            

        }
    }
}

bool search(int value, int values[], int n)
{
    printf("looking through: \n");
    int x;
    for (x = 0; x < n; x++) {
        printf("values[%d] : %d\n", x, values[x]);
    }
    // TODO: implement a searching algorithm
    // Return false if n is non-positive
    if (n < 0) {
        return false;
    }
    
    // Try binary search
    // TODO base case
    
    if (n == 1) {
        if (value ==  values[0]) {
            return true;
        }
        else {
            return false;
        }
    }
    
    // find midpoint
    int midpoint = values[n/2];
    if (value == midpoint) {
        return 1;
    }
    printf("the midpoint is %d\n", midpoint);
    int i;
    int j;
    // Check if value is to the left of midpoint
    if (value < midpoint) {
        printf("value is in the left half\n");
        // if it is, then throw away right half, get the left half, and
        // search through it

        // get left half
       
        int values_copy[n/2];
        int counter = 0;
        for (i = 0; i < n/2; i++) {
            counter += 1;
            values_copy[i] = values[i];
        };
        printf("counter: %d\n", counter);
        search(value, values_copy, n/2 );
    }
    else if (value > values[n/2]) {
        printf("value is in the right half\n");
        //get right half
        printf("size of values_copy: %d\n", n/2);
        int values_copy[n/2 -1];
        int counter = 0;
        for (j = 0; j <  n/2; j++) {
            counter += 1;
            values_copy[j] = values[n/2  + 1 + j];
        };
        printf("counter: %d\n", counter);
        
        // print out values_copy to see
        int z;
        for (z=0;z<n/2;z++) {
            printf("values_copy[%d]: %d \n", z, values_copy[z]);
        }
        search(value, values_copy, n/2 - 1);
    }

}
    


