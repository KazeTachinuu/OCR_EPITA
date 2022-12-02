#pragma once 

void readboard(int board[9][9], char* filename);
void printboard(int board[9][9]);

int checkval(int board[9][9], int i, int j, int v);

int solution(int board[9][9], int i, int j, char* filename);
