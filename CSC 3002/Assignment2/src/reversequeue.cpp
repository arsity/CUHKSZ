/*
 * File: reversequeue.cpp
 * ----------------------
 * This file implements the reversequeue.h interface
 * You need "include" the proper header file and write the function
 * definition.
 */

// TODO

#include <iostream>
#include <queue>
#include <stack>
#include <string>
using namespace std;

/*
 * Function: reverseQueue
 * Usage: reverseQueue(queue);
 * ---------------------------
 * Reverses the order of the elements in a queue.  This
 * implementation does so by using a stack to hold the
 * values as they are being reversed.
 */

// pop all element into a stack
// and pop the stack
void reverseQueue(queue<string> &que) {
  stack<string> sta;
  while (!que.empty()) {
    sta.push(que.front());
    que.pop();
  }
  while (!sta.empty()) {
    que.push(sta.top());
    sta.pop();
  }
}

/*
 * Function: listQueue
 * Usage: listQueue(queue);
 * ------------------------
 * Lists the contents of the queue on the standard output stream.
 */

// pop for list and push it again
void listQueue(queue<string> &que) {
  for (int i = 0; i < que.size(); i++) {
    cout << que.front() << " ";
    que.push(que.front());
    que.pop();
  }
}
