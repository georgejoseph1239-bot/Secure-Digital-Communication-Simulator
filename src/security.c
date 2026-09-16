#include "security.h"

#include <sodium.h>
#include <string.h>


int initialize_security(void)
{
    if (sodium_init() < 0)
    {
        return -1;
    }

    return 0;
}


int derive_security_key(
    const char *password,
    unsigned char *key
)
{
    if (password == NULL || key == NULL)
    {
        return -1;
    }

    /*
     * Derive a fixed 32-byte key from the
     * user's passphrase using BLAKE2b.
     *
     * This keeps the user's password out of
     * the encryption function itself.
     */

    if (crypto_generichash(
            key,
            SECURITY_KEY_SIZE,
            (const unsigned char *)password,
            strlen(password),
            NULL,
            0) != 0)
    {
        return -1;
    }

    return 0;
}


int secure_encrypt(
    const unsigned char *plaintext,
    size_t plaintext_length,
    const unsigned char *key,
    unsigned char *nonce,
    unsigned char *ciphertext,
    size_t *ciphertext_length
)
{
    if (plaintext == NULL ||
        key == NULL ||
        nonce == NULL ||
        ciphertext == NULL ||
        ciphertext_length == NULL)
    {
        return -1;
    }


    /*
     * Generate a fresh random nonce.
     *
     * The nonce does not need to be secret.
     */

    randombytes_buf(
        nonce,
        SECURITY_NONCE_SIZE
    );


    /*
     * crypto_secretbox_easy()
     *
     * Encrypts the message and adds an
     * authentication tag.
     */

    if (crypto_secretbox_easy(
            ciphertext,
            plaintext,
            plaintext_length,
            nonce,
            key) != 0)
    {
        return -1;
    }


    *ciphertext_length =
        plaintext_length +
        SECURITY_TAG_SIZE;


    return 0;
}


int secure_decrypt(
    const unsigned char *ciphertext,
    size_t ciphertext_length,
    const unsigned char *nonce,
    const unsigned char *key,
    unsigned char *plaintext,
    size_t *plaintext_length
)
{
    if (ciphertext == NULL ||
        nonce == NULL ||
        key == NULL ||
        plaintext == NULL ||
        plaintext_length == NULL)
    {
        return -1;
    }


    if (ciphertext_length < SECURITY_TAG_SIZE)
    {
        return -1;
    }


    /*
     * This function first verifies the
     * authentication tag.
     *
     * If the ciphertext was modified,
     * verification fails and plaintext
     * is not accepted.
     */

    if (crypto_secretbox_open_easy(
            plaintext,
            ciphertext,
            ciphertext_length,
            nonce,
            key) != 0)
    {
        return -1;
    }


    *plaintext_length =
        ciphertext_length -
        SECURITY_TAG_SIZE;


    return 0;
}