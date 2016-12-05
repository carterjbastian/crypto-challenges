#include <stdio.h>
#include <stdlib.h>

#include "message_util.h"
#include "message_ops.h"

int main() {
  FILE *fp;
  char *line = NULL;
  size_t linelen= 0;
  fp = fopen("c4_data.txt", "r");

  if (!fp)
    fprintf(stderr, "That was absolutely awful. You can't even open a file\n");

  int count = 0;
  while (getline(&line, &linelen, fp) != -1) { 
    char *encoded = line;
    encoded[linelen - 1] = '\0'; 
    BinaryMessage *m = hex_string_to_message(encoded);
    int len = m->length;

    char *pad = malloc(len + 1);
    pad[len] = '\0';

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
//      if (s > 0) {
        printf("%s \tChar: %c [%f]\tstring: %s", output->data, pad_char, s, encoded);
        //printf("\tCharacter: %c [%f]\n\tResult: %s\n", pad_char, s, output->data);
//      }
      // Clean up the memory
      free_message(output);
      free_message(pad_message);
    }

    free_message(m);
  }
}
