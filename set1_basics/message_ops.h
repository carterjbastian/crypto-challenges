#ifndef MESSAGE_OPS_H_
#define MESSAGE_OPS_H_

#include "message_util.h"

#define ASCII_LOW_VAL 32
#define ASCII_HIGH_VAL 126

BinaryMessage *XOR(BinaryMessage *a, BinaryMessage *b);

int score(BinaryMessage *a);


#endif // MESSAGE_OPS_H_
