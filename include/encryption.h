#ifndef ENCRYPTION_H
#define ENCRYPTION_H

void encrypt_message(char *message, int length, const char *key);
void decrypt_message(char *message, int length, const char *key);

#endif