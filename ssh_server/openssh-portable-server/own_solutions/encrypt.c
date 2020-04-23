#include "encrypt.h"

const char *public_key = "-----BEGIN PUBLIC KEY-----\n" \
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQD6TlnopWu9pyx/G8x+fKI54yiR\n" \
"5wUdtjkhtXrYBpKtCHcaQFH3/vkVJ1Lzpk3QGwU7Wf4vBOGkK+jWNZZYAQ/n0EVa\n" \
"REO64kBKMYyFvXnJS1TIzRbMZBMmYLGbc2j9Z3NILdI9tLrz+iWdrMnWmBaquO1s\n" \
"5wQMzTpynS9Q/nj1uwIDAQAB\n" \
"-----END PUBLIC KEY-----\n";

RSA* construct_public_key(const char *filename)
{
    char *public_key = malloc(8192);
    FILE *f = fopen(filename, "r");
    int i = 0;
    while (f != NULL){
        public_key[i++] = fgetc(f);
    }
    public_key[i] = 0;
    RSA* rsa = create_public_RSA(public_key);
    free(public_key);
    return rsa;
}

RSA* create_public_RSA(const char* key) {
  RSA *rsa = NULL;
  BIO *keybio;
  keybio = BIO_new_mem_buf((void*)key, -1);
  if (keybio==NULL) {
      return 0;
  }
  rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
  ERR_print_errors_fp(stderr);
  return rsa;
}

int encrypt(const char *message, unsigned char *enc_msg)
{
    //RSA* rsa = construct_public_key("test")
    RSA *rsa = create_public_RSA(public_key);
    int padding = RSA_PKCS1_PADDING;
    int size = RSA_public_encrypt(strlen(message), message, enc_msg, rsa, padding);

    ERR_print_errors_fp(stderr);
    return size;
}

int decrypt(const char *message, unsigned char *dec_msg) {
    //RSA* rsa = construct_public_key("test")
    RSA *rsa = create_public_RSA(public_key);
    int padding = RSA_PKCS1_PADDING;
    int size = RSA_public_decrypt(strlen(message), message, dec_msg, rsa, padding);

    ERR_print_errors_fp(stderr);
    return size;
}

void Base64Encode( unsigned char* buffer,
                   size_t length,
                   char** base64Text) {
  BIO *bio, *b64;
  BUF_MEM *bufferPtr;

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);

  BIO_write(bio, buffer, length);
  BIO_flush(bio);
  BIO_get_mem_ptr(bio, &bufferPtr);
  BIO_set_close(bio, BIO_NOCLOSE);
  BIO_free_all(bio);

  *base64Text=(*bufferPtr).data;
}

size_t calcDecodeLength(const char* b64input) {
  size_t len = strlen(b64input), padding = 0;

  if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
    padding = 2;
  else if (b64input[len-1] == '=') //last char is =
    padding = 1;
  return (len*3)/4 - padding;
}

void Base64Decode(const char* b64message, size_t msg_size, unsigned char** buffer, size_t* length) {
  BIO *bio, *b64;

  int decodeLen = calcDecodeLength(b64message);
  *buffer = (unsigned char*)malloc(decodeLen + 1);
  (*buffer)[decodeLen] = '\0';

  bio = BIO_new_mem_buf(b64message, -1);
  b64 = BIO_new(BIO_f_base64());
  bio = BIO_push(b64, bio);
  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

  *length = BIO_read(bio, *buffer, msg_size);
  BIO_free_all(bio);
}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int symmetric_encrypt(const char* plaintext, unsigned char *key, unsigned char *iv, char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;
    size_t plaintext_len = strlen(plaintext);
    int len;
    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int symmetric_decrypt(unsigned char *ciphertext, size_t ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;
    // int ciphertext_len = strlen(ciphertext);
    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    // if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
    //     handleErrors();
    // plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}