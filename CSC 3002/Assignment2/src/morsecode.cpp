/*
 * File: morsecode.cpp
 * -------------------
 * This file implements the morsecode.h interface.
 * You need "#include" the proper header file and write the function
 * definition.
 */

// TODO

#include <map>
#include <queue>
#include <string>
using namespace std;

/*
 * Function: createMorseCodeMap
 * Usage: Map<string> map = createMorseCodeMap();
 * ----------------------------------------------
 * Returns a map in which each uppercase letter is mapped into its
 * Morse code equivalent.
 */

// morsecode map
map<string, string> createMorseCodeMap() {
  map<string, string> map;
  map["A"] = ".-";
  map["B"] = "-...";
  map["C"] = "-.-.";
  map["D"] = "-..";
  map["E"] = ".";
  map["F"] = "..-.";
  map["G"] = "--.";
  map["H"] = "....";
  map["I"] = "..";
  map["J"] = ".---";
  map["K"] = "-.-";
  map["L"] = ".-..";
  map["M"] = "--";
  map["N"] = "-.";
  map["O"] = "---";
  map["P"] = ".--.";
  map["Q"] = "--.-";
  map["R"] = ".-.";
  map["S"] = "...";
  map["T"] = "-";
  map["U"] = "..-";
  map["V"] = "...-";
  map["W"] = ".--";
  map["X"] = "-..-";
  map["Y"] = "-.--";
  map["Z"] = "--..";
  return map;
}

// TODO
/*
 * Function: invertMap
 * Usage: Map<string> inverse = invertMap(map);
 * --------------------------------------------
 * Creates an inverted copy of the specified map in which the values
 * in the original become the keys of the new map and refer back to
 * their associated keys.  Thus, if "abc" is bound to "xyz" in the
 * original map, the inverted map will bind "xyz" to "abc".  If two
 * keys in the original map have the same value, this function will
 * signal an error condition.
 */

// iterate all element
// assign value to key, assign key to value
map<string, string> invertMap(const map<string, string> &Map) {
  map<string, string> morseCodeMap;
  for (auto it : Map) {
    morseCodeMap[it.second] = it.first;
  }
  return morseCodeMap;
}

/*
 * Constant maps: LETTERS_TO_MORSE, MORSE_TO_LETTERS
 * -------------------------------------------------
 * These variables contain maps that convert in each direction between
 * uppercase letters and their Morse Code equivalent.  Because these
 * variables are initialized once and retain their values throughout
 * the lifetime of the program, they are best treated as constants
 * that are shared among the different functions instead of as variables
 * that are passed as parameters.
 */

const map<string, string> LETTERS_TO_MORSE = createMorseCodeMap();
const map<string, string> MORSE_TO_LETTERS = invertMap(LETTERS_TO_MORSE);

/*
 * Function: translateLettersToMorse
 * Usage: string morse = translateLettersToMorse(line);
 * ----------------------------------------------------
 * Translates a string of letters into Morse Code characters separated
 * by spaces.  Characters that don't appear in the table are simply
 * ignored.
 */

// iterate all letter and make it uppercase string
// translate one by one, others are ignored
string translateLettersToMorse(string line) {
  string result;
  for (char ch : line) {
    ch = toupper(ch);
    string str = string(1, ch);
    try {
      result += LETTERS_TO_MORSE.at(str);
      result += " ";
    }
    catch (const exception e) {
      result += " ";
    }
  }
  return result;
}

/*
 * Function: translateMorseToLetters
 * Usage: string letters = translateLettersToMorse(line);
 * ------------------------------------------------------
 * Translates a string in Morse Code into English letters.
 * Because word breaks are not represented in Morse code, the
 * letters in the output will be run together.  The characters
 * of the Morse Code input must be separated by a single space.
 * Any other character in the input is simply ignored.  If there
 * is no English equivalent for the Morse Code character, this
 * function indicates that fact by inserting a question mark (?).
 *
 * Implementation note: To eliminate the special case of the last
 * character in the line, this function begins by adding a space
 * to the end of the input string.
 */

// use " " for seperation
// use "?" for unknown pattern
string translateMorseToLetters(string line) {
  line += " ";
  string result;
  queue<string> morseCodeQueue;
  for (int i = 0; i < line.length() - 1; i++) {
    int loc = line.find(" ", i);
    string morseCode = line.substr(i, loc - i);
    morseCodeQueue.push(morseCode);
    i = loc;
  }
  while (!morseCodeQueue.empty()) {
    try {
      result += MORSE_TO_LETTERS.at(morseCodeQueue.front());
      morseCodeQueue.pop();
    }
    catch (const exception &e) {
      result += "?";
    }
  }
  return result;
}
