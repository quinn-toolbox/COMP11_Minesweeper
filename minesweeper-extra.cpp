/* 
 * minesweeper.cpp 
 *
 * A text-based minesweeper game that runs in the terminal.
 *
 * COMP11 Spring 2018
 * Modified by: Quinn Pham
 * Date: 03/11/2018
 * Reference: looked online to find how to convert int to char
 */

#include <iostream>
#include <time.h> 
#include <cstdlib>

using namespace std;

#ifdef TESTING
#define RANDOMSEED 1
#else
#define RANDOMSEED time(NULL)
#endif

const int DIMEN = 9;
const double PROB = 0.3;
const char MINE = 'x';
const char UNSEEN = '-';

struct Game {
        char board[DIMEN][DIMEN];
        char mines[DIMEN][DIMEN];
        bool game_over;
        int num_mines;
};

Game init_game();
Game populate_plain_board(Game game);
void print_game_board(Game game);
Game populate_mines(Game game);
bool check_if_mine(Game game, int row, int col);
bool valid_input(Game game, int row, int col);
Game num_mines_in_vicinity(Game game, int row, int col);
bool is_it_in_square(int row, int col); 
void print_game_over(Game game);
bool did_i_win(Game game);

int main() {
        srand(RANDOMSEED);
        int num_row;
        int num_col;
        
        Game game = init_game(); 
        game = populate_mines(game);
        while (game.game_over == false){       
                cout << "Enter the row and column of ";
                cout << "the square to uncover (or -0 to give up): ";
                cin >> num_row;
                if (num_row != -0){
                        cin >> num_col;
                        if (valid_input(game, num_row, num_col) == true){
                                if (check_if_mine(game, num_row, num_col)\
                                    == true){
                                        cout << "GAME OVER. "; 
                                        cout << "YOU LOST!" << endl;
                                        game.game_over = true;
                                }
                                else {
                                        game= num_mines_in_vicinity(game,\
                                                          num_row, num_col);
                                        if (did_i_win(game) == true){
                                                game.game_over = true;
                                        }
                                        else{
                                                game.game_over = false;
                                        }
                                }
                        }
                        else {
                                game.game_over = false;
                        }
                }
                else {
                        print_game_over(game);
                        cout << "YOU GAVE UP!" << endl;
                        game.game_over = true;
                }
        }
        return 0;
}

/* 
 * init_game()
 * Purpose: Creates and initializes a Game struct
 * Return value: an initialized Game struct
 */
Game init_game() {
        Game game;
        game.game_over = false;
        game.num_mines = 0;
        game = populate_plain_board(game);
        print_game_board(game);
        return game;
}

/*
 * populate_plain_board()
 * Purpose: initializes game.board by setting all values to UNSEEN
 * Parameter: a Game struct
 * Return value: an updated Game struct
 */
Game populate_plain_board(Game game) {
        for (int i = 0; i < DIMEN; i++) {
                for (int j = 0; j < DIMEN; j++) {
                        game.board[i][j] = UNSEEN;
                }
        }
        return game;
}

/*
 * print_game_board()
 * Purpose: print the game board, with row and column numbers on the sides.
 * Parameter: a Game struct
 */
void print_game_board(Game game) {
        cout << "  ";
        for (int i = 0; i < DIMEN; i++) {
                cout << i << " ";
        }
        cout << endl;

        for (int i = 0; i < DIMEN; i++) {
                cout << i << " ";
                for (int j = 0; j < DIMEN; j++) {
                        cout << game.board[i][j] << " ";
                }
                cout << endl;
        }
}

/*
 * populate_mines()
 * Purpose: puts mines in the board by setting some squares to MINE
 * depending on the random number generated (< PROB)
 * Parameter: a Game struct
 * Return value: an updated Game struct with mines (game.mines)
 */
Game populate_mines(Game game){
        int num_mines = 0;
        for (int i = 0; i < DIMEN; i++){
                for (int j = 0; j < DIMEN; j++){
                        if (rand() % 100 <= PROB * 100){
                                game.mines[i][j] = MINE;
                                num_mines++;
                        }
                }
        }
        game.num_mines = num_mines; // update num_mines in Game struct
        return game;
}

/*
 * check_if_mines()
 * Purpose: checks if the selected row/ col is on a mine
 * Parameter: a Game struct, int row and int col
 * Return value: true or false
 */
bool check_if_mine(Game game, int row, int col) {
        if (game.mines[row][col] == MINE){
                print_game_over(game); // go to game over function
                return true;
        }
        return false;
}

/* 
 * valid_input()
 * Purpose: Checks validity of input without clogging up main()
 * Parameter: a Game struct, int row and int col
 * Return value: true or false
 */
bool valid_input(Game game, int row, int col) { 
        if (row < 0 or row >= DIMEN or col < 0 or col >= DIMEN) {
                cout << "Bad input. Row and col must be";
                cout << " >= 0 and < " << DIMEN << "." << endl;
        }
        else if (game.board[row][col] != UNSEEN) {
                cout << "Bad input. Select another square." << endl;
        }
        else{
                return true; // return true if valid
        }
        return false; // otherwise return false
}

/*
 * num_mines_in_vicinity()
 * Purpose: Checks number of mines in vicinity, print board, return game
 * Parameter: a Game struct, int row and int col
 * Return value: updated game struct
 */
Game num_mines_in_vicinity(Game game, int row, int col){
        int num_mines = 0;
        for (int i = row - 1; i <= row + 1; i++){
                for (int j = col - 1; j <= col + 1; j++){
                        if (is_it_in_square(i, j) == true){
                                if (game.mines[i][j] == MINE){
                                        num_mines++;
                                }
                        }
                }
        }
        
        char num = '0' + num_mines; // converts int to char
        game.board[row][col] = num;
        print_game_board(game);
        return game;
}

/*
 * is_it_in_square()
 * Purpose: Takes i and j (after equations in num_mines_in_vicnity)
 * if i and j are out of bounds, then those squares do not count
 * Parameter: int row and int col (arguments: i and j)
 * Return value: true or false
 */
bool is_it_in_square(int row, int col) {
        if (row >= 0 and row < DIMEN){
                if (col >= 0 and col < DIMEN){
                        return true;
                }
        }
        return false;
}

                                
/* print_game_over()
 * Purpose: Print game board when the game is over
 * Parameter: a Game struct
 * Return value: none
 */
void print_game_over(Game game){
        for (int i = 0; i < DIMEN; i++){
                for (int j = 0; j < DIMEN; j++){
                        if (game.mines[i][j] == MINE){
                                game.board[i][j] = MINE;
                        }
                }
        }
        print_game_board(game); 
}

/* 
 * did_i_win()
 * Purpose: Check if won or not - if won, print game board
 * Parameter: a Game struct
 * Return value: true or false
 */
bool did_i_win(Game game){
        int num_unseen = 0;
        for (int i = 0; i < DIMEN; i++){
                for (int j = 0; j < DIMEN; j++){
                        if (game.board[i][j] == UNSEEN){
                                num_unseen++;
                        }
                }
        }
        // num_unseen == num_mines means that under those unseens are 
        // all mines, so the user has uncovered all non-mine squares
        if  (num_unseen == game.num_mines){ 
                cout << "GAME OVER. YOU WON!" << endl;
                return true;
        }
        return false;
}
