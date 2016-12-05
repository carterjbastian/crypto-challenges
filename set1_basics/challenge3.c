#include <stdio.h>
#include <stdlib.h>

#include "message_util.h"
#include "message_ops.h"

int main() {
  char *encoded = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  BinaryMessage *m = hex_string_to_message(encoded);
  int len = m->length;

  char *pad = malloc(len + 1);
  pad[len] = '\0';

  printf("Starting to iterate through ascii values...\n");
  // Check ascii vals 32-126
  for (int i = 32; i <= 126; i++) {
    char pad_char = (char) i;

    // Build the one-time pad
    for (int j = 0; j < len; j++)
      pad[j] = pad_char;
    
    BinaryMessage *pad_message = plaintext_to_message(pad, len);

    // XOR the original with the potential one-time pad
    BinaryMessage *output = XOR(m, pad_message);

    // Test if the result is possibly right
    double s = score(output);
    if (s > 0)
      printf("Character: %c [%f]\n\tResult: %s\n", pad_char, s, output->data);

    // Clean up the memory
    free_message(output);
    free_message(pad_message);
  }

  printf("Finished iterating.\n");
}
