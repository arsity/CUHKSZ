#include <cstdio>
#include <climits>
#include <queue>
#include <vector>
using namespace std;

struct lengthNodes {
  int length;
  int nodeIndex;
  lengthNodes(int length, int nodeIndex) {
    this->length = length;
    this->nodeIndex = nodeIndex;
  }
};

vector<int> dijkstra(int src, const vector<vector<pair<int, int>>>& graph) {
  vector<int> distance(graph.size(), INT_MAX);
  using my_value = lengthNodes;
  using my_container = vector<my_value>;
  auto my_comp = [](const my_value& t1, const my_value& t2) {
    return t1.length > t2.length;
  };
  priority_queue<my_value, my_container, decltype(my_comp)> priorityQueue{my_comp};
  priorityQueue.push(lengthNodes(0, src));
  while (!priorityQueue.empty()) {
    while (distance[priorityQueue.top().nodeIndex] != INT_MAX) {
      priorityQueue.pop();
      if (priorityQueue.empty())
        return distance;
    }
    auto top = priorityQueue.top();
    priorityQueue.pop();
    distance[top.nodeIndex] = top.length;
    for (const auto& p : graph[top.nodeIndex]) {
      int newDistance = p.second + top.length;
      priorityQueue.push(lengthNodes(newDistance, p.first));
    }
  }
  return distance;
}

int main() {
  int verticesNums{}, edgeNums{};
  scanf("%d %d\n", &verticesNums, &edgeNums);
  vector<vector<pair<int, int>>> graph(verticesNums);
  for (int i = 0; i < edgeNums; i++) {
    int v1{}, v2{}, length{};
    scanf("%d %d %d\n", &v1, &v2, &length);
    graph[v1 - 1].push_back({v2 - 1, length});
    graph[v2 - 1].push_back({v1 - 1, length});
  }
  auto zeroToAll = dijkstra(0, graph);
  auto lastToAll = dijkstra(verticesNums - 1, graph);
  unsigned int res = INT_MAX;
  int smallest = lastToAll[0];
  for (int i = 0; i < verticesNums; ++i) {
    for (auto p : graph[i]) {
      unsigned int sum = zeroToAll[i] + lastToAll[p.first] + p.second;
      if (sum != smallest) {
        res = min(res, sum);
      }
    }
  }
  printf("%d", res);
  return 0;
}