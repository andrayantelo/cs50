#include <stdio.h>
//#include <cs50.h>
#include <stdbool.h>

void sort(int values[], int n);
bool search(int value, int values[], int n);
int find_smaller_between(int values[], int start, int end);

int main(void) {
    int values[4];
    values[0] = 43;
    values[1] = 41;
    values[2] = 40;
    values[3] = 39;
 
    printf("Now going to try to sort them: \n");
    sort(values, 4);
    
     printf("values right now:\n");
    int i;
    for(i = 0; i < 4; i++) {
        printf("values[%d] = %d\n", i, values[i]);
    };
}


void swap(int values[], int smallest_element_index, int current_place_index) {
    // Swap the elements i and j in the values array
   
    int smallest_element = values[smallest_element_index];
    values[smallest_element_index] = values[current_place_index];
    values[current_place_index] = smallest_element;
    return;
    
};

int find_smaller_between(int values[], int start, int end) {
    // Returns the index of the smallest integer between the values in the array values
    // beginning from the index start and ending at index end.
    
    int i;
    int j;
    for (i = start; i < end; i++) {
        
        for (j = start; j < end; j++) {
            if (values[i] > values[j]) {
                // Break out of loop, not the smallest element
                break;
            }
            
        }
        // if you reached the last value
        if (j == end) {
            return i;
        }
    }
};

void sort(int values[], int n)
{
    // Implement an O(n^2) sorting algorithm
    // Let's try selection sort.
    int place_counter = 0;
    int smallest_element_index; 
    
    do{
        // Find smallest element
        smallest_element_index = find_smaller_between(values, place_counter, n);
        
        swap(values, smallest_element_index, place_counter);
        
        place_counter += 1;
    }
    while(place_counter < n);
    
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
