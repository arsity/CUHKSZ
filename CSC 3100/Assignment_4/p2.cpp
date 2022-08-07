#include <iostream>
#include <unordered_set>
using namespace std;

int main() {
  int dataset;
  cin >> dataset;
  for (int i = 0; i < dataset; i++) {
    int size;
    cin >> size;
    unordered_set<int> hashSet(50001);
    for (int j = 0; j < size; j++) {
      int num;
      cin >> num;
      if (hashSet.find(num) == hashSet.end()) {
        printf("%d ", num);
        hashSet.insert(num);
      }
    }
    printf("\n");
  }
  return 0;
}