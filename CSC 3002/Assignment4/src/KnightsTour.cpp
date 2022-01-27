/*
 * File: KnightsTour.cpp
 * ---------------------
 * This program find a knight's tour on an N x M chessboard.
 */

#include "grid.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

/* Function prototypes */

void solveKnightsTour(int n, int m);
void displayBoard(Grid<int> &board);
bool findKnightsTour(Grid<int> &board, int row, int col, int seq, int n, int m);
bool validPosition(Grid<int> &board, int row, int col, int n, int m);
int findAllValidNextStep(Grid<int> &board, int row, int col, int n, int m);
stack<int> nextStepPriority(Grid<int> &board, int row, int col, int n, int m);

/* Test program */

int test2() {
  solveKnightsTour(8, 8);
  return 0;
}

/*
 * Function: solveKnightsTour
 * Usage: solveKnightsTour(n, m);
 * ------------------------------
 * Solves the knight's tour problem for an n x m chessboard.
 */

void solveKnightsTour(int n, int m) {
  Grid<int> board(n, m);
  if (findKnightsTour(board, 0, 0, 1, n, m)) {
    displayBoard(board);
  } else {
    cout << "No tour exists for this board." << endl;
  }
}

/*
 * Function: displayBoard
 * Usage: displayBoard(board);
 * ---------------------------
 * Displays each of the squares in the board along with its sequence
 * number in the tour.
 */

void displayBoard(Grid<int> &board) {
  for (int i = board.numRows() - 1; i >= 0; i--) {
    for (int j = 0; j < board.numCols(); j++) {
      cout << " " << setw(2) << board[i][j];
    }
    cout << endl;
  }
}

/*
 * Function: findKnightsTour
 * Usage: flag = findKnightsTour(board, row, col, seq);
 * ----------------------------------------------------
 * This function looks for a tour on the board, starting at
 * the position indicated by row and col. The seq parameter is
 * the sequence number of this move and is needed to keep
 * track of the steps on the tour. The function returns true
 * if a tour is found and false otherwise.
 *
 * This function begins by checking for three simple cases:
 *
 * 1. The position is off the board.
 * 2. The position has been previously visited.
 * 3. The tour is complete.
 *
 * In the first two cases, there can be no tour that begins
 * from that position on the board. In the last case, the
 * function can immediately return true.
 *
 * If the simple cases do not apply, the function marks the
 * current square and then tries all possible moves. If it
 * finds a tour from the resulting position, the function
 * returns true to the next highest level. If no moves lead
 * to a tour, the function must back out of this move and try
 * again at a higher level of the recursion.
 *
 * The for loops in this implementation are a little tricky.
 * They compute the row and column offsets for the next position by
 * noting that the row and column offsets add to 3 (either +1 and +2
 * or +2 and +1) and you need all four possible sign combinations.
 */

// used for iteration
const int steps[8][2] = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};

// import n, m to generalize
bool findKnightsTour(Grid<int> &board, int row, int col, int seq, int n, int m) {
  stack<vector<int>> previousPosition; // used to save previous position
  stack<stack<int>> previousChoice; // used to save correspondent valid steps for previous position
  while (true) {
    if (validPosition(board, row, col, n, m)) { // verify position
      if (seq == n * m) { //verify successful end
        board[row][col] = seq; //print the last sequence number
        return true;
      } else { // save the position and valid steps
        // continue to move to next position
        previousPosition.push(vector < int > {row, col});
        stack<int> nextStepSequence = nextStepPriority(board, row, col, n, m);
        board[row][col] = seq;
        row += steps[nextStepSequence.top()][0];
        col += steps[nextStepSequence.top()][1];
        nextStepSequence.pop();
        previousChoice.push(nextStepSequence);
        seq++;
      }
    } else { // invalid position, start back tracking
      stack<int> remainSequence = previousChoice.top();
      while (remainSequence.empty()) { // remove positions that have no more choices
        board[previousPosition.top()[0]][previousPosition.top()[1]] = 0;
        seq--;
        previousPosition.pop();
        previousChoice.pop();
        if (previousChoice.empty()) {
          return false;
        }
        remainSequence = previousChoice.top();
      }
      // go to new position and continue
      row = previousPosition.top()[0] + steps[remainSequence.top()][0];
      col = previousPosition.top()[1] + steps[remainSequence.top()][1];
      remainSequence.pop();
      previousChoice.pop();
      previousChoice.push(remainSequence);
    }
  }
}

// list next steps, with the number of further next steps
// ordered with descending sequence
// import n, m to generalize
stack<int> nextStepPriority(Grid<int> &board, int row, int col, int n, int m) {
  vector<vector<int>> possibleMoves(8, vector<int>(2));
  stack<int> result;
  for (int it = 0; it < 8; it++) {
    possibleMoves[it][0] = findAllValidNextStep(board, row + steps[it][0], col + steps[it][1], n, m);
    possibleMoves[it][1] = it;
  }
  sort(possibleMoves.rbegin(), possibleMoves.rend());
  for (auto it : possibleMoves) {
    result.push(it[1]);
  }
  return result;
}

// find number of all valid next steps for a specific position
// import n, m to generalize
int findAllValidNextStep(Grid<int> &board, int row, int col, int n, int m) {
  int sum = 0;
  for (auto step : steps) {
    sum = validPosition(board, row + step[0], col + step[1], n, m) ? sum + 1 : sum;
  }
  return sum;
}

// verify its position
// import n, m to generalize
bool validPosition(Grid<int> &board, int row, int col, int n, int m) {
  return (row >= 0 && row <= n - 1 && col >= 0 && col <= m - 1 && board[row][col] == 0);
}
