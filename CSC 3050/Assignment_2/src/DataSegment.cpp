#include "DataSegment.h"
#include <fstream>
#include <string>
using namespace std;

// extract .data section into a vector
vector<string> extractData(const string& path) {
  vector<string> res{};
  ifstream input(path);
  if (input.is_open()) {
    string line{};
    while (getline(input, line)) {
      res.push_back(line);
    }
    input.close();
    for (auto& s : res) {
      if (s.find('#') != string::npos) {
        s.erase(s.find('#'));
      }
      if (s.find(".data") != string::npos) {
        s.clear();
      } else if (s.find(".text") != string::npos) {
        res.erase(std::find(res.begin(), res.end(), s), res.end());
        break;
      }
    }
    return res;
  } else {
    throw runtime_error("Failed to open" + path);
  }
}

// put static data into memory
int* putData(const vector<string>& cache) {
  char* p = (char*)reserveMemoryPtr + 0x100000;
  for (auto& line : cache) {
    if (!line.empty()) {
      auto pos = std::find_if(line.begin(), line.end(), [&](const auto& item) {
        return item == '.';
      });
      if (pos == line.end()) {
        continue;
      }
      auto end = std::find_if_not(++pos, line.end(), [&](const auto& item) {
        return isalpha(item);
      });
      string type(pos, end);
      if (type == "ascii") {
        auto con_start = std::find_if(line.begin(), line.end(), [&](const auto& item) {
          return item == '"';
        });
        auto con_end = std::find_if(++con_start, line.end(), [&](const auto& item) {
          return item == '"';
        });
        string content(con_start, con_end);
        substituteEscapeSequence(content);
        for (char s : content) {
          *p++ = s;
        }
        while ((intptr_t)p % 4 != 0) {
          p++;
        }
      } else if (type == "asciiz") {
        auto con_start = std::find_if(line.begin(), line.end(), [&](const auto& item) {
          return item == '"';
        });
        auto con_end = std::find_if(++con_start, line.end(), [&](const auto& item) {
          return item == '"';
        });
        string content(con_start, con_end);
        substituteEscapeSequence(content);
        for (auto s : content) {
          *p++ = s;
        }
        *p++ = '\0';
        while ((intptr_t)p % 4 != 0) {
          p++;
        }
      } else if (type == "word") {
        int* ptr = (int*)p;
        auto itr = std::find_if(line.begin(), line.end(), [&](const auto& item) {
          return item == ':';
        });
        while (itr != line.end()) {
          auto con_start = std::find_if(itr, line.end(), [&](const auto& item) {
            return isdigit(item);
          });
          if (con_start == line.end()) {
            break;
          }
          auto con_end = std::find_if_not(con_start, line.end(), [&](const auto& item) {
            return isdigit(item);
          });
          if (*(con_start - 1) == '-') {
            con_start--;
          }
          *ptr++ = stoi(string(con_start, con_end));
          itr = con_end;
        }
        p = (char*)ptr;
      } else if (type == "byte") {
        auto itr = std::find_if(line.begin(), line.end(), [&](const auto& item) {
          return item == ':';
        });
        while (itr != line.end()) {
          auto con_start = std::find_if(itr, line.end(), [&](const auto& item) {
            return isdigit(item);
          });
          if (con_start == line.end()) {
            break;
          }
          auto con_end = std::find_if_not(con_start, line.end(), [&](const auto& item) {
            return isdigit(item);
          });
          if (*(con_start - 1) == '-') {
            con_start--;
          }
          *p++ = (char)stoi(string(con_start, con_end));
          itr = con_end;
        }
      } else if (type == "half") {
        auto* ptr = (short*)p;
        auto itr = std::find_if(line.begin(), line.end(), [&](const auto& item) {
          return item == ':';
        });
        while (itr != line.end()) {
          auto con_start = std::find_if(itr, line.end(), [&](const auto& item) {
            return isdigit(item);
          });
          if (con_start == line.end()) {
            break;
          }
          auto con_end = std::find_if_not(con_start, line.end(), [&](const auto& item) {
            return isdigit(item);
          });
          if (*(con_start - 1) == '-') {
            con_start--;
          }
          *ptr++ = (short)stoi(string(con_start, con_end));
          itr = con_end;
        }
        p = (char*)ptr;
      } else {
        throw runtime_error("Unsupported date type");
      }
    }
  }
  return (int*)p;
}

// substitute '\'&'\n' into '\n'
void substituteEscapeSequence(string& str) {
  auto pos = str.begin();
  pos = std::find_if(pos, str.end(), [&](const auto& item) {
    return item == '\\';
  });
  while (pos != str.end()) {
    switch (*next(pos)) {
      case '\'':
        str.replace(pos, next(pos, 2), "\'");
        break;
      case '\"':
        str.replace(pos, next(pos, 2), "\"");
        break;
      case '\?':
        str.replace(pos, next(pos, 2), "\?");
        break;
      case '\\':
        str.replace(pos, next(pos, 2), "\\");
        break;
      case 'a':
        str.replace(pos, next(pos, 2), "\a");
        break;
      case 'b':
        str.replace(pos, next(pos, 2), "\b");
        break;
      case 'f':
        str.replace(pos, next(pos, 2), "\f");
        break;
      case 'n':
        str.replace(pos, next(pos, 2), "\n");
        break;
      case 'r':
        str.replace(pos, next(pos, 2), "\r");
        break;
      case 't':
        str.replace(pos, next(pos, 2), "\t");
        break;
      case 'v':
        str.replace(pos, next(pos, 2), "\v");
        break;
      default:
        throw runtime_error("Failed to substitute the escape sequence: Unsupported type.");
    }
    pos = std::find_if(pos, str.end(), [&](const auto& item) {
      return item == '\\';
    });
  }
}

