#include <stdio.h>
//#include <cs50.h>
#include <stdbool.h>

void sort(int values[], int n);
bool search(int value, int values[], int n);

int main(void) {
    int values[5];
    values[0] = 20;
    values[1] = 8;
    values[2] = 2;
    values[3] = 15;
    values[4] = 1;
    
    printf("values before sorting: \n");
    int y;
    for (y = 0; y < 5; y++) {
        printf("values: %d\n", values[y]);
    }
    
    sort(values, 5);
    printf("values after sorting: \n");
    for (y = 0; y < 5; y++) {
        printf("values: %d\n", values[y]);
    }
    
    search(3, values, 5);
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
    // TODO: implement a searching algorithm
    // Return false if n is non-positive
    if (n < 0) {
        return false;
    }
    
    // Try binary search
    int half = values[n/2];
    printf("halway point: %d\n", half);
    printf("you gave value: %d\n", value);
    
    // if values is less than midway point, then check the left half
    if (value < values[n/2]) {
        int i;
        for (i = 0; i < n/2; i ++) {
            if (values[i] === value) {
                return 1;
            }
        }
    }
    // otherwise check the right
    else {
        
    }
}

