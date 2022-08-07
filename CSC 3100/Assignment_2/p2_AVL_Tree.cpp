#include <algorithm>
#include <iostream>
using namespace std;

struct Node {
  int key, height, size{};
  Node *left, *right{};

  Node(){};
  Node(int key) {
    this->key = key;
    this->left = this->right = nullptr;
    this->height = this->size = 1;
  }
};
Node* newNode(int key) {
  Node* n = new Node(key);
  return n;
}

int getHeight(Node* n) {
  if (n == nullptr) {
    return 0;
  } else {
    return n->height;
  }
}
int getSize(Node* n) {
  if (n == nullptr) {
    return 0;
  } else {
    return n->size;
  }
}

Node* rightRotate(Node* root) {
  Node* newRoot = root->left;
  Node* moveNode = newRoot->right;
  root->left = moveNode;
  newRoot->right = root;

  root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
  newRoot->height =
      max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;
  root->size = getSize(root->left) + getSize(root->right) + 1;
  newRoot->size = getSize(newRoot->left) + getSize(newRoot->right) + 1;

  return newRoot;
}

Node* leftRotate(Node* root) {
  Node* newRoot = root->right;
  Node* moveNode = newRoot->left;
  root->right = moveNode;
  newRoot->left = root;

  root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
  newRoot->height =
      max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;
  root->size = getSize(root->left) + getSize(root->right) + 1;
  newRoot->size = getSize(newRoot->left) + getSize(newRoot->right) + 1;

  return newRoot;
}

int getBalance(Node* root) {
  if (root == nullptr) {
    return 0;
  } else {
    return getHeight(root->left) - getHeight(root->right);
  }
}
Node* insert(int key, Node* root, unsigned long long& res) {
  if (root == nullptr) {
    return newNode(key);
  }

  if (key < root->key) {
    root->left = insert(key, root->left, res);
    res += (getSize(root->right) + 1);
  } else {
    root->right = insert(key, root->right, res);
  }

  root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
  root->size = getSize(root->left) + getSize(root->right) + 1;

  int balance = getBalance(root);

  if (balance > 1 && key < root->left->key) {
    return rightRotate(root);
  }

  if (balance < -1 && key > root->right->key) {
    return leftRotate(root);
  }

  if (balance > 1 && key > root->left->key) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  if (balance < -1 && key < root->right->key) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

int main() {
  int n, x;
  cin >> n;
  unsigned long long res = 0;
  Node* r = nullptr;
  for (int i = 0; i < n; i++) {
    cin >> x;
    r = insert(x, r, res);
  }
  cout << res;
  return 0;
}
