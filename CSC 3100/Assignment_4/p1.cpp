#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
  Node* left{};
  Node* right{};
  int val{};
  Node() {
    left = nullptr;
    right = nullptr;
    val = 0;
  }
  explicit Node(int val) {
    left = nullptr;
    right = nullptr;
    this->val = val;
  }
};

int symmetricCount(Node* left, Node* right) {
  if (left == nullptr && right == nullptr)
    return 0;
  if ((left == nullptr && right != nullptr) || (left != nullptr && right == nullptr) || left->val != right->val)
    return -1;
  int l = symmetricCount(left->left, right->right);
  if (l == -1)
    return -1;
  int r = symmetricCount(left->right, right->left);
  if (r == -1)
    return -1;
  return l + r + 2;
}

int main() {
  int size;
  cin >> size;
  Node nodeArray[size];
  for (int i = 0; i < size; i++) {
    int num{};
    cin >> num;
    nodeArray[i] = Node(num);
  }
  for (int i = 0; i < size; i++) {
    int left{}, right{};
    cin >> left >> right;
    if (left != -1)
      nodeArray[i].left = &nodeArray[left - 1];
    if (right != -1)
      nodeArray[i].right = &nodeArray[right - 1];
  }
  int res = 1;
  for (auto p : nodeArray) {
    res = max(res, symmetricCount(p.left, p.right) + 1);
  }
  cout << res << endl;
  return 0;
}