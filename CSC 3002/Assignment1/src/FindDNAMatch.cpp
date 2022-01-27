/*
 * File: FindDNAMatch.cpp
 * ----------------------
 * This file implements the FindDNAMatch.h interface.
 */

/* Function definition */

/*TODO*/
#include <iostream>
#include <string>
using namespace std;

string matchingStrand(string strand) {
  for (char &c : strand) {
    switch (c) // inversely complemention
    {
      case 'A':
        c = 'T';
        break;
      case 'T':
        c = 'A';
        break;
      case 'C':
        c = 'G';
        break;
      case 'G':
        c = 'C';
        break;
      default: // error input
        c = 'X';
        break;
    }
  }
  return strand;
}

int findDNAMatch(string s1, string s2, int start = 0) {
  return s2.find(matchingStrand(s1), start);
}

void findAllMatches(string s1, string s2) {
  int p = -1; // ensure start from 0
  while (true) {
    p = findDNAMatch(s1, s2,
                     p + 1); // start from the next of previous stop
    if (p == -1) {
      break; // no more completion and go next line
    } else {
      cout << p << endl;
    }
  }
}