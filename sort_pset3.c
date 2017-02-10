#include <stdio.h>
//#include <cs50.h>
#include <stdbool.h>

void sort(int values[], int n);
bool search(int value, int values[], int n);
int find_smaller_between(int values[], int start, int end);

int main(void) {
    int values[4];
    values[0] = 39;
    values[1] = 40;
    values[2] = 41;
    values[3] = 43;
    //printf("values before sorting: \n");
    //int y;
    //for (y = 0; y < 5; y++) {
    //    printf("values: %d\n", values[y]);
    //}
    
    sort(values, 4);
    //printf("values after sorting: \n");
    //for (y = 0; y < 5; y++) {
    //    printf("values: %d\n", values[y]);
    //}
    
    bool found = search(42, values, 4);
    if (found == true) {
        printf("Found the value\n");
    }
    else {
        printf("Didn't find it: %d\n", found);
    }
    
    printf("Finding smallest element in [5,8,10,2,20]\n");
    int vals[] = {5,8,10,2,20};
    int smallest = find_smaller_between(vals, 0, 5);
    printf("smallest element: %d\n", smallest);
}

int check_against_values(int values[], int value, int n) {
    // check a value against the other values in the array
    int i;
    
    for (i = 0; i < n; i++) {
        printf("checking value: %d against values[%d] = %d\n", value, i, values[i]);
        if (value > values[i]) {
            return false;
        }
    }
    return true;
};

int find_smaller_between(int values[], int start, int end) {
    // Find smallest element in array.
    
    printf("starting at values[%d]\n", start);
    printf("ending at values[%d]\n", end - 1);
    int i;
    int j;
    int smallest_element;
    
    // Take ith element and compare it to all the other elements
    for (i = start; i < end; i++) {
        bool smallest = check_against_values(values, values[i], end);
        if (smallest == true) {
            printf("found smallest: %d\n", values[i]);
            smallest_element = values[i];
            break;
        }
    }
    
    
};
int swap(int values[], int i, int j) {
    // Swap the elements i and j in the values array
    
    
};

void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    // Let's try selection sort.
   
    int smallest_element;
    int place_counter = 0;
    
    // Find smallest element
    smallest_element = find_smaller_between(values, place_counter, n);
    
    //swap smallest with first element (place_counter);
    // increment place_counter and begin again.
    
    
    
/*    // Take ith element and compare it to all the other elements
    for (i = 0; i < n; i++) {
        
        // Starting at the beginning of the array, which place_counter represents.
        for (j = place_counter; j < n; j++) {
            if (values[i] > values[j]) {
                // Break out of loop, not the smallest element
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
*/
}

bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    // Return false if n is non-positive
    if (n <= 0) {
        return false;
    }
    
    // find midpoint
    int midpoint = values[n/2];
    if (value == midpoint) {
        return true;
    }
    
   
    
    // Check if value is to the left of midpoint
    if (value < midpoint) {
      
        // if it is, then throw away right half, get the left half, and
        // search through it

        // get left half
       
        //int values_copy[n/2 - 1];
        int i;
        for (i = 0; i < n/2; i++) {
            values[i] = values[i];
            printf("values[%d] : %d\n", i, values[i]);
        };
        
        return search(value, values, n/2 );
    }
    else if (value > midpoint) {
        // Get the right half.
        //int values_copy[n - (n/2 + 1)];
        int j;
        for (j = 0; j <  n - (n/2 + 1); j++) {
            values[j] = values[n/2  + 1 + j];
            printf("values[%d] : %d\n", j, values[j]);
        };
        
        
      
        // Search through the right half.
        return search(value, values, n - (n/2 + 1));
    }

    return false;
}
