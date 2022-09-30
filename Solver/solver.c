#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 9
#define COLS 9

void readboard(char board[ROWS][COLS], char *filename) {
  int i, j;
  int counti = 0, countj = 0;
  FILE *infile;
  infile = fopen(filename, "r");
  for (i = 0; i < ROWS; i++) {
    countj = 0;
    if (counti == 3 || counti == 6) {
      fscanf(infile, "\n");
    }
    for (j = 0; j < COLS; j++) {

      if (countj == 3 || countj == 6) {
        fscanf(infile, "%*c");
      }
      fscanf(infile, "%c", &board[i][j]);
      countj++;
    }
    fscanf(infile, "\n");
    counti++;
  }
  fclose(infile);
}

void printboard(char board[ROWS][COLS], char *filename) {
  int i, j;
  FILE *infile;
  char *pch = strstr(filename, ".result");
  if (!pch)
    strcat(filename, ".result");
  infile = fopen(filename, "w");
  for (i = 0; i < ROWS; i++) {
    if (i % 3 == 0 && i != 0)
      fprintf(infile, "\n");
    for (j = 0; j < COLS; j++) {
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

int checkval(char board[ROWS][COLS], int i, int j, char v) {
  int x, y;
  for (x = 0; x < 9; x++) {
    if (x != i)
      if (board[x][j] == v)
        return 0;
  }
  for (y = 0; y < 9; y++) {
    if (y != j)
      if (board[i][y] == v)
        return 0;
  }
  for (x = (i / 3) * 3; x < (i / 3) * 3 + 3; x++) {
    for (y = (j / 3) * 3; y < (j / 3) * 3 + 3; y++) {
      if ((x != i) || (y != j))
        if (board[x][y] == v)
          return 0;
    }
  }
  return 1;
}

int solution(char board[ROWS][COLS], int i, int j, char *filename)

{
  char v;
  if (j == 9) {
    j = 0;
    i++;
  }
  if (i == 9) {
    printboard(board, filename);
    return 1;
  }
  if (board[i][j] != '.') {
    if (solution(board, i, j + 1, filename))
      return 0;
  } else {
    for (v = '1'; v <= '9'; v++)
      if (checkval(board, i, j, v)) {
        board[i][j] = v;
        if (solution(board, i, j + 1, filename))
          return 1;
      }
    board[i][j] = '.';
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <filename>)", argv[0]);
    return 1;
    }
  char board[ROWS][COLS];
  readboard(board, argv[1]);
  solution(board, 0, 0, argv[1]);
  return 0;
}
