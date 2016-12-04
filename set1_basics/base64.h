/*
 * Original source: https://gist.github.com/barrysteyn/7308212
 */
#ifndef BASE64_H_
#define BASE64_H_

//Decodes Base64
#include <stdlib.h>

size_t calcDecodeLength(const char* b64input);

int Base64Decode(char* b64message, unsigned char** buffer, size_t* length);

int Base64Encode(const unsigned char* buffer, size_t length, char** b64text);

#endif // BASE64_H_
