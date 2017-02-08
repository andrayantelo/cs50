#include <stdio.h>
#include <cs50.h>

void sort(int values[], int n);
int main(void) {
    int values[5];
    values[0] = 2;
    values[1] = 1;
    values[2] = 10;
    values[3] = 0;
    values[4] = 5;
    sort(values, 5);
}



void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    // Let's try selection sort.
    
    // Find smallest unsosrted element
    int i;
    int j;
    //int smallest_element;
    int k;
    int values_copy[n];
    for (k = 0; k < n; k++) {
        values_copy[k] = values[k];
    }
    int place_counter = 0;
    for (i = 0; i < n; i++) {
        //Take ith element and compare it to all the other elements
        printf("currently checking value: %d\n", values[i]);
        for (j = 0; j < n; j++) {
            if (values_copy[i] > values_copy[j]) {
                //break out of loop
                j = n;
            }
            
        }
        if (j == n) {
            printf("smallest element: %d\n", values_copy[i]);
            // Swap places between the smallest element and the element currently in the first position
            values[i] = values[place_counter];
            values[place_counter] = values_copy[i];
            place_counter += 1;
            // have to move to the next value in values.. i mean after the recently sorted smallest element.
            //i = place_counter;
        }
    }
    int z;
    for(z=0;z<n;z++) {
        printf("values: %d\n", values[z]); 
    }

   
}
