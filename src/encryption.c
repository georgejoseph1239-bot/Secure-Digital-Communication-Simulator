#include "encryption.h"
#include <string.h>

void encrypt_message(char *message, int length, const char *key)
{
    int key_length = strlen(key);

    if (key_length == 0)
        return;

    for (int i = 0; i < length; i++)
    {
        message[i] = message[i] ^ key[i % key_length];
    }
}

void decrypt_message(char *message, int length, const char *key)
{
    // XOR encryption is reversible.
    encrypt_message(message, length, key);
}