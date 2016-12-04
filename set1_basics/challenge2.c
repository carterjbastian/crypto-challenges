#include <stdio.h>

#include "message_util.h"
#include "message_ops.h"

int main() {
  BinaryMessage *a = hex_string_to_message("1c0111001f010100061a024b53535009181c");
  BinaryMessage *b = hex_string_to_message("686974207468652062756c6c277320657965");

  BinaryMessage *c = XOR(a, b);
  printf("Plaintext: %s\n", c->data);
  printf("Output: %s\n", message_to_hex(c));
  printf("Intent: 746865206b696420646f6e277420706c6179\n");
}
