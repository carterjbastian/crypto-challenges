#include <stdio.h>

#include "message_util.h"

int main() {
  char *hex_s = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  char *correct_out = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

  char *out = message_to_base64(hex_string_to_message(hex_s));

  printf("Output: %s\n", out);
  printf("Intent: %s\n", out);
}
