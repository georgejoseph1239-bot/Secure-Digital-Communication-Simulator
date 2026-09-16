#ifndef SECURITY_H
#define SECURITY_H

#include <stddef.h>

#define SECURITY_KEY_SIZE 32
#define SECURITY_NONCE_SIZE 24
#define SECURITY_TAG_SIZE 16

int initialize_security(void);

int derive_security_key(
    const char *password,
    unsigned char *key
);

int secure_encrypt(
    const unsigned char *plaintext,
    size_t plaintext_length,
    const unsigned char *key,
    unsigned char *nonce,
    unsigned char *ciphertext,
    size_t *ciphertext_length
);

int secure_decrypt(
    const unsigned char *ciphertext,
    size_t ciphertext_length,
    const unsigned char *nonce,
    const unsigned char *key,
    unsigned char *plaintext,
    size_t *plaintext_length
);

#endif