#include <iostream>
#include <vector>
using namespace std;

unsigned long long countInversions(vector<int>& nums) {
  auto size = nums.size();
  if (size == 1) {
    return 0;
  } else {
    vector<int> former(nums.begin(), nums.begin() + size / 2);
    vector<int> latter(nums.begin() + size / 2, nums.end());
    auto countF = countInversions(former);
    auto countL = countInversions(latter);
    unsigned long long i = 0, j = 0, k = 0, res = 0;
    while (k < size) {
      if (i < former.size() && j < latter.size()) {
        if (former[i] <= latter[j]) {
          nums[k++] = former[i++];
        } else {
          res += (former.size() - i);
          nums[k++] = latter[j++];
        }
      } else if (i == former.size()) {
        nums[k++] = latter[j++];
      } else {
        nums[k++] = former[i++];
      }
    }
    return res + countF + countL;
  }
}

int main() {
  int size, k;
  cin >> size;
  vector<int> nums;
  for (int i = 0; i < size; i++) {
    cin >> k;
    nums.push_back(k);
  }
  cout << countInversions(nums) << endl;
  return 0;
}