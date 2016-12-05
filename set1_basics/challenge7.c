/*
 * Leaning heavily on the OpenSSL wiki page accessible at:
 * https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 */
#include <stdio.h>
#include <string.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "message_util.h"
#include "base64.h"

int decrypt(unsigned char *encrypted, int msg_len, unsigned char *key,
    unsigned char *decrypted);

void handleErrors(void);

int main() {
  /* 128 bit key */
  unsigned char *key = (unsigned char *) "YELLOW SUBMARINE";

  /* Build Message to be decrypted */
  FILE *fp = fopen("b64_7.data", "r");

  if (!fp)
    fprintf(stderr, "That was absolutely awful. You can't even open a file\n");

  // Get the size of the file
  // TODO(carter): place a size limit
  fseek(fp, 0, SEEK_END);
  long msg_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // Copy the file data into a string
  char *msg_data = calloc(msg_size + 1, 1);
  fread((void *)msg_data, 1, msg_size, fp);

  // Convert from base64 to a plaintext message
  BinaryMessage *msg = base64_string_to_message(msg_data);

  /* Set up Buffer for decrypted text */
  unsigned char *decryptedtext = calloc(msg->length * 2, 1);
  int decryptedtext_len;

  /* Initialize the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  /* Decryption */
  decryptedtext_len = decrypt(msg->data, msg->length, key, decryptedtext);

  printf("Encrypted text:\n%s\n", (char *)decryptedtext);

  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();

  return 0;
}

int decrypt(unsigned char *encrypted, int msg_len, unsigned char *key,
    unsigned char *decrypted) {
  /* Build context */
  EVP_CIPHER_CTX *ctx;
  int len;
  int decrypted_len;

  /* Initialize context */
  if (!(ctx = EVP_CIPHER_CTX_new()))
    handleErrors();

  /* Initialize Decryption */
  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    handleErrors();

  /* Do the actual decryption */
  if (1 != EVP_DecryptUpdate(ctx, decrypted, &len, encrypted, msg_len))
    handleErrors();
  decrypted_len = len;

  /* Final decryption */
  if (1 != EVP_DecryptFinal_ex(ctx, decrypted + len, &len))
    handleErrors();
  decrypted_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return decrypted_len;
}

// Taken directly from the OpenSSL wiki
void handleErrors(void) {
  ERR_print_errors_fp(stderr);
  abort();
}
