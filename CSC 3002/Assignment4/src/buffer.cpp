/*
 * File: buffer.cpp (list version without dummy cell)
 * --------------------------------------------------
 * This file implements the EditorBuffer class using a linked
 * list to represent the buffer.
 */

#include "buffer.h"
#include <iostream>
#include <string>
using namespace std;

/*
 * Implementation notes: EditorBuffer constructor
 * ----------------------------------------------
 * This function initializes an empty editor buffer, represented
 * as a doubly linked list.  In this implementation, the ends of
 * the linked list are joined to form a ring, with the dummy cell
 * at both the beginning and the end.  This representation makes
 * it possible to implement the moveCursorToEnd method in constant
 * time, and reduces the number of special cases in the code.
 */

EditorBuffer::EditorBuffer() {
  start = cursor = new Cell;
  start->next = start;
  start->prev = start;
}

/*
 * Implementation notes: EditorBuffer destructor
 * ---------------------------------------------
 * The destructor must delete every cell in the buffer.  Note
 * that the loop structure is not exactly the standard idiom for
 * processing every cell within a linked list, because it is not
 * legal to delete a cell and later look at its next field.
 */

EditorBuffer::~EditorBuffer() {
  // set a pointer
  Cell* cp = start;
  // delete all cells
  while (cp != nullptr) {
    Cell* nextCell = cp->next;
    delete cp;
    cp = nextCell;
  }
}

/*
 * Implementation notes: cursor movement
 * -------------------------------------
 * In a doubly linked list, each of these operations runs in
 * constant time.
 */

// not at end and move
void EditorBuffer::moveCursorForward() {
  if (cursor->next != start) {
    cursor = cursor->next;
  }
}

// not at start and move
void EditorBuffer::moveCursorBackward() {
  if (cursor->prev != start) {
    cursor = cursor->prev;
  }
}

// initialize to start
void EditorBuffer::moveCursorToStart() {
  cursor = start;
}

// initialize to end
void EditorBuffer::moveCursorToEnd() {
  cursor = start->prev;
}

/*
 * Implementation notes: insertCharacter, deleteCharacter
 * ------------------------------------------------------
 * This code is much like that used for the traditional linked
 * list except that more pointers need to be updated.
 */

void EditorBuffer::insertCharacter(char ch) {
  // create a new cell
  Cell* newCell = new Cell;
  // set the pointers of the new cell
  newCell->ch = ch;
  newCell->next = cursor->next;
  newCell->prev = cursor;
  // set relevant other cell's pointers
  cursor->next->prev = newCell;
  cursor->next = newCell;
  cursor = newCell;
  // initialize the pointer
  newCell = nullptr;
}

void EditorBuffer::deleteCharacter() {
  // create a pointer
  Cell* wantedDelete = cursor->next;
  // unlink relevant cells
  cursor->next->next->prev = cursor;
  cursor->next = cursor->next->next;
  // delete and initialize the pointer
  delete wantedDelete;
  wantedDelete = nullptr;
}

/*
 * Implementation notes: getText and getCursor
 * -------------------------------------------
 * The getText method uses the standard linked-list pattern to loop
 * through the cells in the linked list.  The getCursor method counts
 * the characters in the list until it reaches the cursor.
 */

string EditorBuffer::getText() const {
  // initialize C++ string and pointer
  string result = "";
  Cell* cptr;
  // set start point and iteration
  cptr = start->next;
  while (cptr != start) {
    result += string(1, cptr->ch);
    cptr = cptr->next;
  }
  return result;
}

int EditorBuffer::getCursor() const {
  // initialize the result
  int index = 0;
  // iteration
  for (Cell* cp = start; cp != cursor; cp = cp->next) {
    index++;
  }
  return index;
}
