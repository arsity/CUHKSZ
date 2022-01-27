/*
 * File: combinatorics.cpp
 * -----------------------
 * This file implements the combinatorics.h interface.
 */

/* Function definition */

/*TODO*/

// Define permutations function
int permutations(int n, int k) {
  int result = 1;

  // Due to mathematical subtraction, product from n to (n-k+1)
  for (int multiplies = n; multiplies > (n - k); multiplies--) {
    result *= multiplies;
  }
  return result;
}

// Define combinations function
int combinations(int n, int k) {
  // It could be written as permutations(n,k) / (k!)
  return permutations(n, k) / permutations(k, k);
}