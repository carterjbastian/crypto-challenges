#include <stdio.h>

#include "message_util.h"

int main() {
  char *hex_txt = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  char *txt = "I'm killing your brain like a poisonous mushroom";
  char *base64_txt = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

  BinaryMessage *hex_m = hex_string_to_message(hex_txt);
  BinaryMessage *txt_m = plaintext_to_message(txt);
  BinaryMessage *b64_m = base64_string_to_message(base64_txt);

  printf("hex_m: %s [%d]\n", hex_m->data, hex_m->length);
  printf("txt_m: %s [%d]\n", txt_m->data, txt_m->length);
  printf("b64_m: %s [%d]\n", b64_m->data, b64_m->length);

  printf("Correct Hex:\t%s\n", hex_txt);
  printf("Converted Hex:\t%s\n", message_to_hex(txt_m));

  printf("Correct Base64:\t%s\n", base64_txt);
  printf("Converted Base64:\t%s\n", message_to_base64(txt_m));

  free_message(hex_m);
  free_message(txt_m);
  free_message(b64_m);
}
