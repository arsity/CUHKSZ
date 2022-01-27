/*
 * File: bigint.cpp
 * ----------------
 * This file implements the bigint.h interface.
 */

#include "bigint.h"
#include "error.h"
#include <cctype>
#include <string>
using namespace std;

/*
 * Implementation notes: BigInt constructor
 * ----------------------------------------
 * The code for this constructor offers a minimal implementation
 * that matches what we would expect on an exam.  In a more
 * sophisticated implementation, it would make sense to include
 * a test to avoid storing leading zeros in the linked list.  In
 * this implementation, calling BigInt("00042") creates a
 * BigInt with a different internal representation than
 * BigInt("42"), which is probably a bad idea.
 */

BigInt::BigInt(string str) {
  string str1;
  if (str.empty()) {  // exclude special condition
    cerr << "\"\" is INVALID.";
  } else {
    // remove leading zeros
    long long digit = 0;
    while (str[digit] == 0 && digit != str.length() - 1) {
      digit++;
    }
    str1 = str.substr(digit, str.length()
        - digit);  // slice for valid remains
    // construct the linked list
    Cell* previousCell = nullptr;
    for (long long it = 0; it < str1.length(); it++) {
      Cell* currentCell = new Cell;
      currentCell->finalDigit = stoi(str1.substr(it, 1));
      currentCell->leadingDigits = previousCell;
      previousCell = currentCell;
      start = currentCell;
    }
    previousCell = nullptr;
  }
}

/*
 * Implementation notes: BigInt destructor
 * ---------------------------------------
 * The code for the destructor is similar to that of the other
 * classes that contain a linked list.  You need to store the
 * pointer to the next cell temporarily so that you still have
 * it after you delete the current cell.
 */

BigInt::~BigInt() {
  // use pointer to delete cell to cell
  Cell* wantDelete = start;
  while (wantDelete != nullptr) {
    Cell* nextCell = wantDelete->leadingDigits;
    delete wantDelete;
    wantDelete = nextCell;
  }
  start = nullptr;
}

// implementation of copy constructor
// use deep copy strategy
BigInt::BigInt(const BigInt &b) {
  Cell* initial = new Cell;
  initial->finalDigit = b.start->finalDigit;
  initial->leadingDigits = nullptr;
  start = initial;
  Cell* cursor = start;
  for (Cell* b2ptr = b.start->leadingDigits; b2ptr != nullptr;
       b2ptr = b2ptr->leadingDigits) {
    Cell* constructCell = new Cell;
    constructCell->finalDigit = b2ptr->finalDigit;
    constructCell->leadingDigits = nullptr;
    cursor->leadingDigits = constructCell;
    cursor = constructCell;
  }
}

/*
 * Implementation notes: toString
 * ------------------------------
 * This method could also be written as a wrapper method that
 * calls a recursive function that creates the reversed string
 * one character at a time.
 */

// extract each digit data and convert to string
string BigInt::toString() const {
  string result;
  for (Cell* currentCell = start; currentCell != nullptr;
       currentCell = currentCell->leadingDigits) {
    result.insert(0, to_string(currentCell->finalDigit));
  }
  return result;
}

/*
 * Implementation notes: operator+ and operator*
 * ------------------------------
 * Implement operator+ and operator*, make BigInt support addition and
 * multiplication.
 */

// override of operator+ for BigInt
BigInt BigInt::operator+(const BigInt &b2) const {
  // construct the least significant bit to make result stable
  Cell* b1ptr = start->leadingDigits;
  Cell* b2ptr = b2.start->leadingDigits;
  int sum = start->finalDigit + b2.start->finalDigit;
  bool carry = sum / 10;
  BigInt result(to_string(sum % 10));
  Cell* cursor = result.start;
  // iteration for other digits
  while (b1ptr != nullptr || b2ptr != nullptr || carry) {
    if (b1ptr != nullptr && b2ptr != nullptr) {  // both pointers are still in range
      Cell* newDigit = new Cell;
      newDigit->finalDigit =
          carry ? b1ptr->finalDigit + b2ptr->finalDigit + 1 :
              b1ptr->finalDigit + b2ptr->finalDigit;
      newDigit->leadingDigits = nullptr;
      carry = newDigit->finalDigit / 10;
      newDigit->finalDigit %= 10;
      cursor->leadingDigits = newDigit;
      cursor = newDigit;
      b1ptr = b1ptr->leadingDigits;
      b2ptr = b2ptr->leadingDigits;
    } else if (b1ptr != nullptr) {  // b2 pointer is out of range
      Cell* newDigit = new Cell;
      newDigit->finalDigit =
          carry ? b1ptr->finalDigit + 1 : b1ptr->finalDigit;
      newDigit->leadingDigits = nullptr;
      carry = newDigit->finalDigit / 10;
      newDigit->finalDigit %= 10;
      cursor->leadingDigits = newDigit;
      cursor = newDigit;
      b1ptr = b1ptr->leadingDigits;
    } else if (b2ptr != nullptr) {  // b1 pointer is out of range
      Cell* newDigit = new Cell;
      newDigit->finalDigit =
          carry ? b2ptr->finalDigit + 1 : b2ptr->finalDigit;
      newDigit->leadingDigits = nullptr;
      carry = newDigit->finalDigit / 10;
      newDigit->finalDigit %= 10;
      cursor->leadingDigits = newDigit;
      cursor = newDigit;
      b2ptr = b2ptr->leadingDigits;
    } else {  // both pointers are out of range but still carry not saved
      Cell* newDigit = new Cell;
      newDigit->finalDigit = 1;
      newDigit->leadingDigits = nullptr;
      cursor->leadingDigits = newDigit;
      carry = false;
    }
  }
  return result;
}

// override of operator* for BigInt
BigInt BigInt::operator*(const BigInt &b2) const {
  // calculate the least significant digit first to make stable
  Cell* b1ptr = start;
  Cell* b2ptr = b2.start;
  int multiply = b1ptr->finalDigit * b2ptr->finalDigit;
  int carry = multiply / 10;
  BigInt result(to_string(multiply % 10));
  Cell* cursor = result.start;
  int count = 1;
  // consider b1 * b2, that is, each digit of b1 is multiplied by each
  // digit of b2
  for (b2ptr; b2ptr != nullptr; b2ptr = b2ptr->leadingDigits) {
    BigInt buffer("0");
    Cell* bufferCursor = buffer.start;
    for (b1ptr = start->leadingDigits; b1ptr != nullptr;
         b1ptr = b1ptr->leadingDigits) {  // digit * digit
      Cell* headDigit = new Cell;
      headDigit->leadingDigits = nullptr;
      headDigit->finalDigit =
          b1ptr->finalDigit * b2ptr->finalDigit + carry;
      carry = headDigit->finalDigit / 10;
      headDigit->finalDigit %= 10;
      bufferCursor->leadingDigits = headDigit;
      bufferCursor = headDigit;
    }
    if (carry) {  // add a digit to show carry if necessary
      Cell* headDigit = new Cell;
      headDigit->leadingDigits = nullptr;
      headDigit->finalDigit = carry;
      carry = 0;
      bufferCursor->leadingDigits = headDigit;
      bufferCursor = headDigit;
    }
    for (int it = 1; it < count; it++) {  // add zeros to get actual value
      Cell* lastZero = new Cell;
      lastZero->finalDigit = 0;
      lastZero->leadingDigits = buffer.start;
      buffer.start = lastZero;
    }
    result = result + buffer;  // add them up
    count++;
  }
  return result;
}

// override of operator= for BigInt
// use deep copy strategy
BigInt BigInt::operator=(const BigInt &b2) {
  Cell* initial = new Cell;
  start = initial;
  initial->finalDigit = b2.start->finalDigit;
  initial->leadingDigits = nullptr;
  Cell* cursor = start;
  for (Cell* b2ptr = b2.start->leadingDigits; b2ptr != nullptr;
       b2ptr = b2ptr->leadingDigits) {
    Cell* newCell = new Cell;
    newCell->leadingDigits = nullptr;
    newCell->finalDigit = b2ptr->finalDigit;
    cursor->leadingDigits = newCell;
    cursor = newCell;
  }
  return *this;
}
