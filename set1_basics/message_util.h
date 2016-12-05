#ifndef MESSAGE_UTIL_H_
#define MESSAGE_UTIL_H_

typedef struct BinaryMessage {
  unsigned char *data;
  int length;
} BinaryMessage;

BinaryMessage *hex_string_to_message(char *hex_string);

BinaryMessage *base64_string_to_message(char *base64_string);

BinaryMessage *plaintext_to_message(char *plaintext, int length);

char *message_to_hex(BinaryMessage *);

char *message_to_base64(BinaryMessage *);

void free_message(BinaryMessage *);

#endif
