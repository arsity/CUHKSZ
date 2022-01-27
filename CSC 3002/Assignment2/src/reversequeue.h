#ifndef REVERSEQUEUE_H
#define REVERSEQUEUE_H

// TODO
#include <queue>
#include <string>
using namespace std;

/* Function prototypes */

/*
 * Function: reverseQueue
 * Usage: reverseQueue(queue);
 * ---------------------------
 * Reverses the order of the elements in a queue.  This
 * implementation does so by using a stack to hold the
 * values as they are being reversed.
 */

void reverseQueue(queue<string> &queue);

/*
 * Function: listQueue
 * Usage: listQueue(queue);
 * ------------------------
 * Lists the contents of the queue on the standard output stream.
 */

void listQueue(queue<string> &queue);

#endif // REVERSEQUEUE_H
