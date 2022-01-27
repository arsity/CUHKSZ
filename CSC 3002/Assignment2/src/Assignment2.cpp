/*
 * File: Assignment.cpp
 * ----------------------
 * This file tests the Problem 1, 2 and 3.
 */

#include "calendar.h"
#include "morsecode.h"
#include "reversequeue.h"
#include <iostream>
#include <queue>
#include <string>
// TODO

#include "console.h"
using namespace std;

int main() {
  /*******************Problem 1******************/
  /********************Part 1********************/
  /* a sample INPUT: 2012
   * The OUTPUT: Enter a year: 2012
   *             JANUARY has 31 days.
   *             FEBRUARY has 29 days.
   *             ...... //(You need print all of them)
   *             DECEMBER has 31 days.
   */

  // TODO
  string yearStr;
  cout << "Enter a year: ";
  getline(cin, yearStr);
  int year = stoi(yearStr);
  for (int i = 1; i <= 12; i++) {
    cout << monthToString(Month(i)) << " has "
         << daysInMonth(Month(i), year) << " days." << endl;
  }
  cout << "--------------------------------------------------\n";

  /*******************Problem 1******************/
  /********************Part 2********************/
  /* Sample: Date moonLanding1(20, JULY, 1969);
   * OUTPUT: moonLanding1 = 20-Jul-1969
   * Sample: Date moonLanding2(JULY, 20, 1969);
   * OUTPUT: moonLanding2 = 20-Jul-1969
   */
  Date moonLanding1(20, JULY, 1969);
  Date moonLanding2(JULY, 20, 1969);
  cout << moonLanding1.toString() << endl;
  cout << moonLanding2.toString() << endl;
  cout << "--------------------------------------------------\n";

  // TODO

  /*******************Problem 1******************/
  /*******************For Part 3*****************/
  /* Sample: Date moonLanding(20, JULY, 1969);
   *         Date kennedyAssassination(NOVEMBER, 22, 1963);
   *         Date newYearsEve(DECEMBER, 31, 2011);
   *         Date inaugurationDay(21, JANUARY, 2013);
   *         Date electionDay(6, NOVEMBER, 2012);
   *
   * OUTPUT: moonLanding = 20-Jul-1969
   *         kennedyAssassination = 22-Nov-1963
   *         moonLanding < kennedyAssassination = false
   *         moonLanding > kennedyAssassination = true
   *         moonLanding == kennedyAssassination = false
   *         moonLanding == moonLanding = true
   *         inaugurationDay - electionDay = 76
   *         New Year's Eve = 31-Dec-2011
   *         New Year's Day = 1-Jan-2012
   *         6-Nov-2012 //Print days from election day to
   * inauguration day. 7-Nov-2012
   *         ...... //(You need print all of them)
   *         21-Jan-2013
   */

  // TODO
  Date moonLanding(20, JULY, 1969);
  Date kennedyAssassination(NOVEMBER, 22, 1963);
  Date newYearsEve(DECEMBER, 31, 2011);
  Date inaugurationDay(21, JANUARY, 2013);
  Date electionDay(6, NOVEMBER, 2012);
  cout << moonLanding.toString() << endl;
  cout << kennedyAssassination.toString() << endl;
  cout << boolalpha;
  cout << (moonLanding < kennedyAssassination) << endl;
  cout << (moonLanding > kennedyAssassination) << endl;
  cout << (moonLanding == kennedyAssassination) << endl;
  cout << (moonLanding == moonLanding) << endl;
  cout << (inaugurationDay - electionDay) << endl;
  cout << newYearsEve.toString() << endl;
  cout << ((newYearsEve + 1).toString()) << endl;
  cout << endl;
  for (Date i = electionDay; i <= inaugurationDay; i++) {
    cout << i.toString() << endl;
  }
  cout << "--------------------------------------------------\n";

  /*********************Problem 2*******************/
  /* Using enqueue method to obtain a queue containing: Genesis
   * Exodus Leviticus Numbers Deuteronomy List the queue Reverse the
   * queue List the queue A sample OUTOUT: The queue contains:
   * Genesis Exodus Leviticus Numbers Deuteronomy The queue contains:
   * Deuteronomy Numbers Leviticus Exodus Genesis
   */
  queue<string> testQueue;
  testQueue.push("Genesis");
  testQueue.push("Exodus");
  testQueue.push("Leviticus");
  testQueue.push("Numbers");
  testQueue.push("Deuteronomy");
  listQueue(testQueue);
  cout << endl;
  reverseQueue(testQueue);
  listQueue(testQueue);
  cout << endl;
  cout << "--------------------------------------------------\n";

  // TODO

  /*********************Problem 3*******************/
  cout << "Morse code translator" << endl;
  /* A sample translator:
   *                     > SOS
   *                     ... --- ...
   *                     > ... --- ...
   *                     SOS
   */
  cout << "SOS" << endl;
  string morseResult = translateLettersToMorse("SOS");
  cout << morseResult << endl;
  cout << translateMorseToLetters(morseResult) << endl;

  return 0;
}
