#include <cstdio>
#include <climits>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

struct lengthNode {
  unsigned long distance;
  pair<int, int> nodes;
  lengthNode(unsigned long distance, pair<int, int>& nodes) {
    this->distance = distance;
    this->nodes = nodes;
  }
};

void mergeAintoB(int indexAval, int indexBval, vector<unsigned int>& status) {
  for (auto& n : status) {
    if (n == indexAval)
      n = indexBval;
  }
}

int main() {
  int verticesNums, leastLength;
  scanf("%d %d", &verticesNums, &leastLength);
  vector<pair<int, int>> coordinates{};
  for (int i = 0; i < verticesNums; ++i) {
    int x, y;
    scanf("%d %d", &x, &y);
    coordinates.emplace_back(x, y);
  }
  using my_valuetype = lengthNode;
  using my_container = vector<my_valuetype>;
  auto my_comp = [](const my_valuetype& p1, const my_valuetype& p2) { return p1.distance > p2.distance; };
  priority_queue<my_valuetype, my_container, decltype(my_comp)> priorityQueue{my_comp};
  for (int i = 0; i < verticesNums - 1; i++) {
    for (int j = i + 1; j < verticesNums; j++) {
      long euclidDis =
          pow(coordinates[i].first - coordinates[j].first, 2) + pow(coordinates[i].second - coordinates[j].second, 2);
      if (euclidDis >= leastLength) {
        auto k = pair<int, int>(i, j);
        priorityQueue.push(lengthNode(euclidDis, k));
      }
    }
  }
  vector<unsigned int> status(verticesNums, 0);
  auto k = priorityQueue.top();
  priorityQueue.pop();
  status[k.nodes.first] = 1;
  status[k.nodes.second] = 1;
  unsigned int nums = 2;
  unsigned long res = k.distance;
  while (!priorityQueue.empty()) {
    auto p = priorityQueue.top();
    priorityQueue.pop();
    if (status[p.nodes.first] == 0 && status[p.nodes.second] == 0) {
      status[p.nodes.first] = nums;
      status[p.nodes.second] = nums++;
      res += p.distance;
    } else if (status[p.nodes.first] == 0 && status[p.nodes.second] != 0) {
      status[p.nodes.first] = status[p.nodes.second];
      res += p.distance;
    } else if (status[p.nodes.first] != 0 && status[p.nodes.second] == 0) {
      status[p.nodes.second] = status[p.nodes.first];
      res += p.distance;
    } else if (status[p.nodes.second] != status[p.nodes.first]) {
      mergeAintoB(max(status[p.nodes.first], status[p.nodes.second]),
                  min(status[p.nodes.first], status[p.nodes.second]),
                  status);
      res += p.distance;
    }
  }
  for (auto n : status) {
    if (n != 1) {
      printf("%d", -1);
      return 0;
    }
  }
  printf("%lu", res);
  return 0;
};