#ifndef MESSAGE_OPS_H_
#define MESSAGE_OPS_H_

#include "message_util.h"

#define ASCII_LOW_VAL 32
#define ASCII_HIGH_VAL 126

BinaryMessage *XOR(BinaryMessage *a, BinaryMessage *b);

int score(BinaryMessage *a);

BinaryMessage *repeating_string_key(BinaryMessage *a, char *key);

int hamming_weight(unsigned int n);
int hamming_distance(unsigned char *a, unsigned char *b, int count);

#endif // MESSAGE_OPS_H_
