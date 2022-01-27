/*
 * File: BanishLetters.cpp
 * -----------------------
 * This file implements the BanishLetters.h interface
 */

/* Function definition */

/*TODO*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int BanishLetters() {
  string infile, outfile, banword, line;
  cout << "Input file (Absolute Path Required): ";
  getline(cin, infile); // input file directory
  cout << "Output file (Absolute Path Required): ";
  getline(cin, outfile); // output file directory
  cout << "Letters to banish: ";
  getline(cin, banword); // banword string
  fstream in, out;
  in.open(infile.c_str(), ios::in); // open input file
  out.open(outfile.c_str(),
           fstream::out | ios_base::trunc); // open output file
  bool remove = false;
  while (getline(in, line)) // read line by line
  {
    for (char c : line) // read every character
    {
      remove = false;
      for (char banLetter :
          banword) // compare the whole banword string
      {
        if ((c == tolower(banLetter)) ||
            (c == toupper(banLetter))) // consider both uppercase
          // and lowercase
        {
          remove = true;
        }
      }
      if (!remove) // not in banword string and output
      {
        out << c;
      }
    }
    out << endl;
  }
  in.close();
  out.close();
  return 0;
}
