#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include "message_util.h"
#include "message_ops.h"


const double LETTER_FREQ[26] = {
  8.34, 1.54, 2.73, 4.14, 12.60, 2.03, 1.92, 6.11, 6.71, 0.23, 0.87, 4.24,
  2.53, 6.80, 7.70, 1.66, 0.09, 5.68, 6.11, 9.37, 2.85, 1.06, 2.34, 0.20,
  2.04, 0.06,
};

BinaryMessage *XOR(BinaryMessage *a, BinaryMessage *b) {
  // Ensure messages are compatible
  if (a->length != b->length) {
    fprintf(stderr, "Cannot XOR unequally-sized messages\n");
    return NULL;
  }
  
  // XOR the two messages' data into a new string
  int len = a->length;
  char *c_data = malloc(len + 1);
  c_data[len] = '\0';

  // XOR character-wise
  for (int i = 0; i < len; i++)
    c_data[i] = (char) (a->data[i] ^ b->data[i]);

  // Make a new message to hold the data
  // TODO (carter): this can break if xoring => a null byte...
  BinaryMessage *c = plaintext_to_message(c_data); 

  if (c->length < a->length) {
    //fprintf(stderr, "Encountered a null byte error\n");
    free_message(c);
    return NULL;
  }

  return c;
}

int score(BinaryMessage *a) {
  if (!a)
    return 0;

  double score = 1000;

  // Penalize non-printable characters  
  for (int i = 0; i < a->length; i++) {
    int byteval = (int) a->data[i];
    if ((byteval < ASCII_LOW_VAL) || (byteval > ASCII_HIGH_VAL))
      score -= 150;
  }

  int *letter_count = calloc(26, sizeof(int));
  int count = 0;
  int space_count = 0;
  int expected_spaces = a->length / 6;

  for (int i = 0; i < a->length; i++) {
    char c = a->data[i];
    if (isalpha(c)) {
      count++;
      int idx = (int) tolower(c) - (int) 'a';
      letter_count[idx]++;
    } else if (c == ' ') {
      space_count++;
    }
  }

  // Penalize poor frequency distribution
  for (int i = 0; i < 26; i++) {
    score -= fabs(LETTER_FREQ[i] - ((double) letter_count[i] / (double)count));
  }
  
  // Penalize poor word-length (constant is arbitrary)
  score -= 350 * abs(expected_spaces - space_count);

  return score;
}


BinaryMessage *repeating_string_key(BinaryMessage *a, char *key) {
  unsigned char *new_string = calloc(a->length + 1, 1);
  
  int idx = 0;
  int key_len = strlen(key);

  for (int i = 0; i < a->length; i++) {
    new_string[i] = (char) (a->data[i] ^ key[idx]);
    idx = (idx + 1) % key_len;
  }

  BinaryMessage *encrypted = malloc(sizeof(BinaryMessage));

  encrypted->data = new_string;
  encrypted->length = a->length;

  return encrypted;
}
