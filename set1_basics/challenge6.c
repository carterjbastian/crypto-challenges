#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "message_util.h"
#include "message_ops.h"

int main() {
  FILE *fp;
  fp = fopen("b64.data", "r");

  if (!fp)
    fprintf(stderr, "That was absolutely awful. You can't even open a file\n");

  // Get the size of the file
  // TODO(carter): place a size limit
  fseek(fp, 0, SEEK_END);
  long msg_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // Copy the file data into a string
  char *msg_data = calloc(msg_size + 1, 1);
  fread((void *)msg_data, 1, msg_size, fp);

  // Find the length of the key by finding the value in [2,40] which has the
  // lowest hamming distance
  int best_keysize = 0;
  float best_distance = INFINITY;

  for (int keysize = 2; keysize < 40; keysize++) {
    float sample_data[3] = {0, 0, 0};

    // Take 3 samples and average them
    for (int i = 0; i < 2; i++) {
      // Compare two consecutive blocks of length keysize
      char *sample1 = msg_data + (i*keysize);
      char *sample2 = msg_data + (i*keysize) + keysize;

      int curr_distance = hamming_distance(sample1, sample2, keysize);
      float normalized_dist = (float) curr_distance / keysize;

      sample_data[i] = normalized_dist;
    }
    float avg_dist = (sample_data[0] + sample_data[1] + sample_data[2]) / 3.0;

    //printf("Keysize: %d\tDistance: %f\n", keysize, avg_dist);

    // Store the best keysize seen so far
    if (avg_dist < best_distance) {
      best_keysize = keysize;
      best_distance = avg_dist;
    }
  }

  printf("\nBest Result: Keysize = %d, Distance = %f\n", best_keysize, best_distance);
}
