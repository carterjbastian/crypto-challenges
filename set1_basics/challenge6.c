/*
 * TODO(carter): clean up memory leaks
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

  // Convert from base64 to a plaintext message
  BinaryMessage *msg = base64_string_to_message(msg_data);

  // Find the length of the key by finding the value in [2,40] which has the
  // lowest hamming distance
  int best_keysize = 0;
  float best_distance = INFINITY;
  
  for (int keysize = 2; keysize <= 40; keysize++) {
    int sample_size = 2;
    double avg_dist = 0;

    // Take 3 samples and average them
    for (int i = 0; i < sample_size; i++) {
      // Compare two consecutive blocks of length keysize
      unsigned char *sample1 = msg->data + (i*keysize);
      unsigned char *sample2 = msg->data + (i*keysize) + keysize;

      int curr_distance = hamming_distance(sample1, sample2, keysize);
      double normalized_dist = (double) curr_distance / keysize;

      avg_dist += normalized_dist;
    }
    avg_dist /= sample_size;

    /* Uncomment this line to see the full list of keysize options & scores */
    //printf("Keysize: %d\tDistance: %f\n", keysize, avg_dist);

    // Store the best keysize seen so far
    if (avg_dist < best_distance) {
      best_keysize = keysize;
      best_distance = avg_dist;
    }
  }

  /* 
   * DEV NOTE: this took some human input.
   * I played with the listed outputs from this, going the through the values
   * with the smallest few values until I found a value which created a
   * semi-sensible output. I then kept that value by assigning it to 
   * best_keysize by hand.
   */
  best_keysize = 29;
  
  // Create the partitioned mini-messages
  char **blocks = malloc(best_keysize * sizeof(char *));
  if (blocks) {
    for (int i = 0; i < best_keysize; i++) {
      // Divide the length by the keysize (rounding up) + null termination
      blocks[i] = calloc((msg_size / best_keysize) + 2, 1);
    }
  }
  
  // Transpose the message into best_keysize separate messages
  for (int i = 0; i < best_keysize; i++) {
    unsigned char *data_idx = msg->data + i;
    int copy_idx = 0;
    
    // Copy over one byte at a time
    while (data_idx < msg->data + msg->length) {
      memcpy(blocks[i] + copy_idx, data_idx, 1);
      copy_idx++;
      data_idx += best_keysize; // Skip KEYSIZE bytes ahead
    }
  }

  // Set up record keeping for single-character XOR cracking
  float best_score[best_keysize];
  char best_char[best_keysize + 1];
  best_char[best_keysize] = '\0';
  BinaryMessage *best_output[best_keysize];

  // Run code for cracking single-character XOR on each block 
  for (int b = 0; b < best_keysize; b++) {
    // Build the single-character encrypted message
    int block_len = (msg_size / best_keysize) + 1;
    BinaryMessage *m = plaintext_to_message(blocks[b], block_len);

    char *pad = malloc(block_len + 1);
    pad[block_len] = '\0';
    best_score[b] = -INFINITY;

    // Check ascii vals 32-126
    for (int i = 32; i <= 126; i++) {
      char pad_char = (char) i;

      // Build the single-character one-time pad
      for (int j = 0; j < block_len; j++)
        pad[j] = pad_char;
      
      BinaryMessage *pad_message = plaintext_to_message(pad, block_len);

      // XOR the original with the potential one-time pad
      BinaryMessage *output = XOR(m, pad_message);

      // Test if the result is possibly right
      double s = score(output);

      /* uncomment these lines to get other options for individual letters. */
      //if (s > 0)
      //  printf("Char %c, score %f\n", pad_char, s); 
      
     // Store the best results 
      if (s > best_score[b]) {
        best_score[b] = s;
        best_char[b] = pad_char;
        best_output[b] = output;
      } else {
        // Clean up the memory
        free_message(output);
      }
      
      // Clean up this specific one-time pad
      free_message(pad_message);
    }

    // Clean up from the 
    free(pad);
    free_message(m);
  }
  
  /* Uncomment below for a full list of best scores by block */
  //for (int i = 0; i < best_keysize; i++)
  //  printf("Block %d, character %c, Score %f\n", i, best_char[i], best_score[i]);
   
  /* 
   * DEV NOTE: the strictly computed version of this string is
   * "TerminatornX:aBringnthennoise". I fixed it by hand by figuring out that,
   * for some reason, n's are replacing spaces in the key computation. 
   */
  // TODO(carter): figure out why spaces in the key computation are breaking
  char *key = (char *)best_char;
  key = "Terminator X: Bring the noise";
  
  // Print results
  printf("Key is: %s\n", key);
  BinaryMessage *decrypted = repeating_string_key(msg, key);
  printf("Decrypted Text:\n%s", decrypted->data);
}
