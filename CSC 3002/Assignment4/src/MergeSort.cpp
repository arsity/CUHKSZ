/*
 * File: MergeSort.cpp
 * -------------------
 * This file implements the merge sort algorithm using arrays rather
 * than vectors.
 */

#include <iostream>
using namespace std;

/* Function prototypes */

void sort(int array[], int n);
void printArray(int array[], int n);
void merge(int arr[], int arr1[], int arr2[], int arr1Size, int arr2Size);

/* Test program */

int test3() {
  int array[] = {56, 25, 37, 58, 95, 19, 73, 30};
  sort(array, 8);
  printArray(array, 8);
  return 0;
}

/*
 * Function: sort
 * Usage: sort(array, n);
 * ----------------------
 * Sorts the first n elements of the vector into increasing order using
 * the merge sort algorithm, which consists of the following steps:
 *
 * 1. Copy the array into an array allocated on the stack.
 * 2. Divide the new array into two halves by manipulating the indices.
 * 3. Sort each half of the array recursively
 * 4. Merge the two halves back into the original one.
 */

void sort(int array[], int n) {
  if (n <= 1) // directly return
  {
    return;
  }
  // separate into 2 sub arrays
  int arr1[n / 2];
  int arr2[n - n / 2];
  for (int i = 0; i < n; i++) {
    if (i < n / 2) {
      arr1[i] = array[i];
    } else {
      arr2[i - n / 2] = array[i];
    }
  }
  // recursively continue
  sort(arr1, n / 2);
  sort(arr2, n - n / 2);
  array[n] = 0; // initialization and then merge
  merge(array, arr1, arr2, n / 2, n - n / 2);
}

// merge function
void merge(int arr[], int arr1[], int arr2[], int arr1Size, int arr2Size) {
  // define iterators
  int it = 0;
  int arr1it = 0;
  int arr2it = 0;
  // start merging
  while (true) {
    if (arr1it < arr1Size && arr2it < arr2Size) // both pointers are not to end
    {
      if (arr1[arr1it] < arr2[arr2it]) {
        arr[it++] = arr1[arr1it++];
      } else {
        arr[it++] = arr2[arr2it++];
      }
    } else if (arr1it >= arr1Size && arr2it < arr2Size) // arr1it is to end
    {
      arr[it++] = arr2[arr2it++];
    } else if (arr2it >= arr2Size && arr1it < arr1Size) // arr2it is to end
    {
      arr[it++] = arr1[arr1it++];
    } else // both are to end and return
    {
      return;
    }
  }
}
/*
 * Function: printArray
 * Usage: printArray(array, n);
 * ----------------------------
 * Prints the elements of the array on a single line with the elements
 * enclosed in braces and separated by commas.
 */

void printArray(int array[], int n) {
  cout << "{ ";
  for (int i = 0; i < n; i++) {
    if (i > 0)
      cout << ", ";
    cout << array[i];
  }
  cout << " }" << endl;
}
