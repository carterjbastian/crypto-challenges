#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message_util.h"
#include "base64.h"

BinaryMessage *hex_string_to_message(char *hex_string) {
  // Every 2 characters encode one byte
  int len = (strlen(hex_string) + 1) / 2;
  BinaryMessage *message = malloc(sizeof(BinaryMessage));

  // Allocate enough space for each character + null termination (zeroed out)
  unsigned char *bin_data = calloc(len + 1, 1);

  // Convert hex string into bytes and form the data
  char *pointer = hex_string;
  char buffer[3] = {0, 0, 0};
  for (int i = 0; i < len; i++) {
    buffer[0] = *pointer++;
    buffer[1] = *pointer++;
    unsigned char byte = (unsigned char) strtoul(
        (const char *)&buffer, NULL, 16);
    bin_data[i] = byte;
  }

  // Create message structure
  message->data = bin_data;
  message->length = len;

  return message;
}

BinaryMessage *base64_string_to_message(char *base64_string) {
  char *decoded;
  size_t message_length;

  Base64Decode(base64_string, (unsigned char **)(&decoded), &message_length);
  return plaintext_to_message(decoded, message_length);
}

// TODO(carter): fix this so null terminator doesn't ruin everything.
BinaryMessage *plaintext_to_message(char *plaintext, int length) {
  int len = length;
  BinaryMessage *message = malloc(sizeof(BinaryMessage));

  // Allocate enough space for each character + null termination (zeroed out)
  unsigned char *bin_data = calloc(len + 1, 1);
  
  // Copy the data into the new location
  memcpy((void *)bin_data, (void *)plaintext, len);

  // Create the message structure
  message->data = bin_data;
  message->length = len;

  return message;
}

char *message_to_hex(BinaryMessage *m) {
  char buffer[3];
  buffer[2] = '\0';

  // Hex representation will have twice as many digits as there are characters
  int hex_len = m->length* 2;
  char *hexstring = calloc(hex_len + 1, 1);
  
  int hex_count = 0;
  for (int i = 0; i < m->length; i++) {
    sprintf((char *)(&buffer), "%02x", m->data[i]);
    // Copy over the hex characters
    hexstring[hex_count++] = buffer[0];
    hexstring[hex_count++] = buffer[1];
  }

  return hexstring;
}

char *message_to_base64(BinaryMessage *m) {
  char *base64_encoded;
  Base64Encode(m->data, m->length, &base64_encoded);
  return base64_encoded;
}

void free_message(BinaryMessage *m) {
  if (!m)
    return;

  free(m->data);
  free(m);
}
