#include <iostream>
#include <unordered_map>
using namespace std;

int main() {
  int n, target;
  cin >> n >> target;
  unordered_map<int, int> hashMap;
  int i = 0;
  while (i < n) {
    int curNum = 0;
    cin >> curNum;
    int toFind = target - curNum;
    if (hashMap.find(toFind) == hashMap.end()) {
      hashMap.insert({curNum, i++});
    } else {
      cout << hashMap.at(toFind) + 1 << ' ' << i + 1 << endl;
      return 0;
    }
  }
  return 1;
}