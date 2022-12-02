#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//solved sudoku board
char solvedBoard[9][9] = {
    {'5', '3', '4', '6', '7', '8', '9', '1', '2'},
    {'6', '7', '2', '1', '9', '5', '3', '4', '8'},
    {'1', '9', '8', '3', '4', '2', '5', '6', '7'},
    {'8', '5', '9', '7', '6', '1', '4', '2', '3'},
    {'4', '2', '6', '8', '5', '3', '7', '9', '1'},
    {'7', '1', '3', '9', '2', '4', '8', '5', '6'},
    {'9', '6', '1', '5', '3', '7', '2', '8', '4'},
    {'2', '8', '7', '4', '1', '9', '6', '3', '5'},
    {'3', '4', '5', '2', '8', '6', '1', '7', '9'}};



//Generate a random valid sudoku board
void generate(char board[9][9], int difficulty) {

    int row, col;
    int i = 0;
    //copy solved board to board
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            board[row][col] = solvedBoard[row][col];
        }
    }
    while (i < difficulty) {
        row = rand() % 9;
        col = rand() % 9;
        if (board[row][col] != '.') {
            board[row][col] = '.';
            i++;
        }
        }
    }

     
    

void printboard(char board[9][9], char *filename) {
  int i, j;
  FILE *infile;
  char *pch = strstr(filename, ".gen");
  if (!pch)
    strcat(filename, ".gen");
  infile = fopen(filename, "w");
  for (i = 0; i < 9; i++) {
    if (i % 3 == 0 && i != 0)
      fprintf(infile, "\n");
    for (j = 0; j < 9; j++) {
      // printf("%c ", board[i][j]);
      fprintf(infile, "%c", board[i][j]);
      if (j == 2 || j == 5)
        fprintf(infile, " ");
    }
    // printf("\n");
    fprintf(infile, "\n");
  }
  // printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <difficulty>\n", argv[0]);
        return EXIT_FAILURE;}
    char board[9][9];
    if (atoi(argv[2]) < 17 || atoi(argv[2]) > 81) {
        printf("Difficulty must be between 17 and 81\n");
        return EXIT_FAILURE;
    }
    generate(board, atoi(argv[2]));
    printboard(board, argv[1]);
    return EXIT_SUCCESS;
}