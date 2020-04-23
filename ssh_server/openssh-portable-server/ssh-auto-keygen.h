#ifndef __SSH_AUTO_KEYGEN_H__
#define __SSH_AUTO_KEYGEN_H__

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

void do_gen_all_hostkeys();
const char *generate_public_private_keys(const char **old_public_key, const char **new_public_key);
char* signMessage(const char* privateKey, const char* plainText);
size_t calcDecodeLength(const char* b64input);
int RSASign( RSA* rsa,
              const unsigned char* Msg,
              size_t MsgLen,
              unsigned char** EncMsg,
              size_t* MsgLenEnc);
RSA* createPrivateRSA(const char* key);

#endif
