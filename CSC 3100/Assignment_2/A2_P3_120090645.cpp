#include <iostream>
#include <unordered_map>
using namespace std;

struct lr {
  int left = 0;
  int right = 0;
  lr() {}
  lr(int left) { this->left = left; }
  lr(int left, int right) {
    this->left = left;
    this->right = right;
  }
};

int main() {
  int count;
  cin >> count;
  unordered_map<int, lr> map{};
  map.insert({1, lr()});
  for (int i = 2; i <= count; i++) {
    int x, p;
    cin >> x >> p;
    if (p) {
      if (map[x].right) {
        map.insert({i, lr(x, map[x].right)});
        map[map[x].right].left = i;
      } else {
        map.insert({i, lr(x)});
      }
      map[x].right = i;
    } else {
      if (map[x].left) {
        map.insert({i, lr(map[x].left, x)});
        map[map[x].left].right = i;
      } else {
        map.insert({i, lr(0, x)});
      }
      map[x].left = i;
    }
  }
  cin >> count;
  while (count != 0) {
    int x;
    cin >> x;
    if (map[x].left != 0 && map[x].right != 0) {
      map[map[x].left].right = map[x].right;
      map[map[x].right].left = map[x].left;
    } else if (map[x].left != 0) {
      map[map[x].left].right = 0;
    } else {
      map[map[x].right].left = 0;
    }
    map.erase(x);
    count--;
  }
  int num = map.begin()->first;
  while (map[num].left != 0) {
    num = map[num].left;
  }
  while (num != 0) {
    cout << num << ' ';
    num = map[num].right;
  }
  return 0;
}