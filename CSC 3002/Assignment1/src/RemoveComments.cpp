/*
 * File: RemoveComments.cpp
 * ------------------------
 * This file implements the RemoveComments.h interface.
 */

/* Function definition */

/*TODO*/
#include <iostream>
#include <string>
using namespace std;

void RemoveComments(istream &is, ostream &os) {
  string line;
  bool inComment = false;   // to mark /* & */
  while (getline(is, line)) // read line by line
  {
    int beginner = 0; // consider no */ as default
    bool noMarkInLine = true;
    for (int i = 0; i < line.length() - 1; i++) // read from 0 to n-1
    {
      if (line[i] == '/' && !inComment) {
        switch (line[i + 1]) {
          case '/': // "//"comment all leftover in line, cout
            // previous code
            os << line.substr(beginner, i - beginner) << endl;
            i = line.length();
            noMarkInLine = false;
            break;
          case '*': // the start of comment, cout its previous code
            os << line.substr(beginner, i - beginner);
            inComment = true;
            noMarkInLine = false;
            i++; // reduce unecessary increment
            break;
          default:
            break;
        }
      } else if (line.substr(i, 2) == "*/" &&
          inComment) // escape from comment, and state where it
        // ends
      {
        beginner = i + 2;
        inComment = false;
        i++; // reduce unecessary increment
      }
    }
    if (noMarkInLine && !inComment) // cout line with no comment marks
    {
      os << line.substr(beginner) << endl;
    }
  }
}