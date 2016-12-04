#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message_ops.h"
#include "message_util.h"

int main() {
  char *data = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
  BinaryMessage *original = plaintext_to_message(data);
  BinaryMessage *encrypted = repeating_string_key(original, "ICE");

  printf("Output: %s\n", message_to_hex(encrypted));
  printf("Intent: 0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f\n");
}
