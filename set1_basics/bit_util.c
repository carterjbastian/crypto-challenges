#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implement's Kernighan's algorithm for population count
int hamming_weight(unsigned int n) {
  int c;

  for (c = 0; n; n = n & (n-1))
    c++;
  
  return c;
}

int hamming_distance(char *a, char *b, int count) {
  int total_distance = 0;

  // the number of differing bits = hamming weight of a XOR b
  for (int i = 0; i < count; i++)
    total_distance += hamming_weight((unsigned int)a[i] ^ (unsigned int)b[i]);
  return total_distance;
}

int main() {
  char *str1 = "this is a test";
  char *str2 = "wokka wokka!!!";
  int len = strlen(str1);

  int dist = hamming_distance(str1, str2, len);
  printf("Computed distance: %d\n", dist);
}
