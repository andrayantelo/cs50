/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;
// blank tile
int blank_tile = 0;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();
    
    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();
        
        
        
        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    
    
    // close log
    fclose(file);

    // success
    return 0;
    
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    
    printf("Initialiing board...\n");
    printf("dimension: %d\n", d);
    
    int start_number = d*d - 1;
    int total_filled_tiles = d*d - 1;
    int one_location[2];
    int two_location[2];
    
    // Iterate over rows
    int i;
    int j;
    for (i = 0; i < d; i++) {
        // Iterate over columns
        for (j = 0; j < d; j++) {
            if (start_number == 2) {
                two_location[0] = i;
                two_location[1] = j;
            }
            else if (start_number == 1) {
                one_location[0] = i;
                one_location[1] = j;
            }
            board[i][j] = start_number;
            //printf("tile at board[%d][%d]  = %d\n", i, j, board[i][j]);
            start_number -= 1;
            
        }
    }
    //swap tiles 1 and 2 if odd number of tiles
    if (total_filled_tiles % 2 != 0) {
        printf("odd number of tiles\n");
        board[one_location[0]][one_location[1]] = 2;
        board[two_location[0]][two_location[1]] = 1;
    }
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    
    // Iterate over rows 
    int i;
    int j;
    for(i = 0; i < d; i++) {
        // Iterate over columns
        for(j = 0; j < d; j++) {
            if (board[i][j] == blank_tile) {
                    printf(" _");
            }
            else if (board[i][j] < 10) {
                printf("%2d", board[i][j]);
            }
            else {
                printf("%d", board[i][j]);
            }
            if (j == d - 1) {
                printf("\n");
            }
            else{
                printf(" ");
            }
        }
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    // Find the tile location.
    
    int tile_row;
    int tile_col;
    int blank_row;
    int blank_col;
    int i;
    int j;
    
    // Get the tile locations.
    // Iterate over rows
    for(i = 0; i < d; i++) {
        // Iterate over columns 
        for (j = 0; j < d; j++) {
            if (board[i][j] == tile) {
                tile_row = i;
                tile_col = j;
            }
            else if (board[i][j] == blank_tile) {
                blank_row = i;
                blank_col = j;
            }
        }
    }
    
    int right_edge = d - 1;
    int left_edge = 0;
    int top_edge = 0;
    int bottom_edge = d - 1;
    // Check if move is legal
    // if the blank is at the left top corner
    if (blank_col == left_edge && blank_row == top_edge) {
        // check if tile is to the right or below
        if ((tile_row == 0 && tile_col == 1) || (tile_row == 1 && tile_col == 0)) {
            // Move is legal
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
    }
    // if tile is bottom left corner
    else if (blank_row == bottom_edge && blank_col == left_edge) {
        // Check if tile is to the right or above
        if ((tile_row == bottom_edge && tile_col == 1) || (tile_row == bottom_edge - 1 && tile_col == 0)) {
            // move is legal
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
    }
    
    // If tile is at the bottom right corner
    else if (blank_row == bottom_edge && blank_col == right_edge) {
        // Check if tile is to the left or above
        if ((tile_row == bottom_edge && tile_col == right_edge - 1) || (tile_row == bottom_edge - 1 && tile_col == right_edge)) {
            // Legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
    }
    // If tile is at the top right corner
    else if (blank_row == 0 && blank_col == right_edge) {
        // Check if tile is to the left or below
        if ((tile_row == 0 && tile_col == right_edge - 1) || (tile_row == 1 && tile_col == right_edge)) {
            // Legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
    }
    // If tile is in top row
    else if (blank_row == 0) {
        // check if it is to the left, right, or below
        if (tile_row == 0 && (tile_col == blank_col + 1 || tile_col == blank_col - 1)) {
            // Legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
        else if (tile_row == 1 && tile_col == blank_col) {
            // Legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
    }
    // If tile is bottom row
    else if (blank_row == bottom_edge) {
        // check if it is to the left or right
        if (tile_row == bottom_edge && (tile_col == blank_col - 1 || tile_col == blank_col + 1)) {
            // legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
        // check if above
        else if (tile_row == bottom_edge - 1 && tile_col == blank_col) {
            // legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
    }
    // if the blank tile is at left edge ( but not a corner)
    else if (blank_col == left_edge) {
        // check if it is to the right 
        if (tile_col == 1 && tile_row == blank_row) {
            // legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
        // check if above or below
        else if (tile_col == 0 && (tile_row == blank_row - 1 || tile_row == blank_row + 1)) {
            // legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
    }
    // if the blank tile is at the right edge
    else if (blank_col == right_edge) {
        // check if tile is to the left
        if (tile_col == (right_edge - 1) && tile_row == blank_row) {
            // legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
        // check if above or below
        else if (tile_col == right_edge && (tile_row == blank_row - 1 || tile_row == blank_row + 1)) {
            // legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
    }
    // Last case, the tile is in one of the middle spots
    // need to check if the tile is above, below, to the right or left of the blank tile.
    else {
        // check if the tile is above or below
        if (tile_col == blank_col && (tile_row == blank_row - 1 || tile_row == blank_row + 1)) {
            // legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
        // check if tile is right or left
        else if (tile_row == blank_row && (tile_col == blank_col - 1 || tile_col == blank_col + 1)) {
            // legal move
            board[blank_row][blank_col] = tile;
            board[tile_row][tile_col] = blank_tile;
            return true;
        }
    }
    
    // If it didn't meet any of the conditions
    printf("tiles didn't meet any of the conditions\n");
    return false;
    
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO
    
    // Iterate over the whole board and check that it is in ascending order
    
    int i;
    int j;
    
    // Iterate over rows
    int current_tile = 0;
    for (i = 0; i < d; i++) {
        // Iterate over columns
        for (j = 0; j < d; j++) {
            // Check here. 
            if (i == d - 1 && j == d - 1) {
                printf("last tile\n");
                break;
            }
            if (board[i][j] < current_tile) {
                printf("failed at: board[%d][%d] = %d\n", i, j, board[i][j]);
                printf("It's not in ascending order\n");
                return false;
            }
            current_tile = board[i][j];
        }
        
    }
    printf("We have left the loop\n");
    return true;
}
