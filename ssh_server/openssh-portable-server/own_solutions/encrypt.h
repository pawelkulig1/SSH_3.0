#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <assert.h>
#include <string.h>

int encrypt(const char *message, unsigned char *enc_msg);
RSA* create_public_RSA(const char* key);
RSA* construct_public_key(const char *filename);
void Base64Encode( unsigned char* buffer, size_t length, char** base64Text, BUF_MEM **ptr);
void Base64Decode(const char* b64message, unsigned char** buffer, size_t* length);

int symmetric_encrypt(const char* plaintext, unsigned char *key, unsigned char *iv, char *ciphertext);
int symmetric_decrypt(unsigned char *ciphertext, size_t ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);

#endif