#include <iostream>
#include <cstring>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <assert.h>

const char* privateKey = "-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIG4gIBAAKCAYEArzDGSrlZWqmTFN+RKdZg3S/gHkdLKXny6TrZheEHUAxDlr8k\n" \
"NcbCzGXgV1WPCMFbBM9DSzdq6jqljXY6vEbFJ1gMN524Il6n0U0mZdKQBKFcIVFw\n" \
"JxXRztQyNUjQO3hRRTDf+ZDlWDqy8KjXivVk9x4t3h5TVV52fOlQ1tWsv4HdgwRz\n" \
"umrHlpIk8pacvTyj71+NE17csy4IdQrdoIzYDtJU++8OHwkzO69mvFPkTag1ZjQX\n" \
"Mebj9bDn3M/zeQfVFyadI2ZuN4/VwHJTQIPThv4f81HlsF+yn71+RSfOPgnQ8Cra\n" \
"UG5h5FSZg096NwqmIGJPFBuCQDYYSJu8WhgZOlGTFzG9iqvm0JYKa0qQdUVr5Cuc\n" \
"DGMwXcCxCxhFIacwaQcxfAQCX6MBiMsolAqRb2re2erYFoavVo93y2UgIZA/5MXT\n" \
"SpZounRiqN6LWtEgYHaLlA7dPmbRGoa+/iIRtS+E7LJHHEnLlgPJ+ym5vx4acmd6\n" \
"N98ps1BuGRR4JFS7AgMBAAECggGAZ3LKtJcsP1AA7QNzKZxQiGvb0KVSqcQ7/yLk\n" \
"DZ0qf0cuphZqYGLk5mOGDKwWERp0D6v/1at8s6E73wOgfUrg4nGojTLR52GzgPEo\n" \
"qlmi90j9KSN4sWu9/37nwvUjGOrcOpo4t5qrAuVugpOA66oqG44zkYSgurlUCECR\n" \
"nrqADnm1/sajO8IP50aXELSFC22CdDgdCwIqGpglIE/xPPvOHTpwOnAYC5WYLjdN\n" \
"2AlFt+GK3rYFuE7Z71uuHoKhB6fqX2MnT2PeZF9FK9I0Vg6teNohAe1pxbqCqmlw\n" \
"zM9SFV/kurvuAOkoH3eQxMDDivHJtlCaUHOml6EqD5n+JxU86rUvJb6x8m6eAkD3\n" \
"RzFWu/gLcuWhg/pJ0aybDROfKfP4hFcZKR8LtMacV8sq37EqfJCCsU/po0zviKwX\n" \
"PM7nx6v31ywhgsyejS9HJMF2Ino5BD36d4nWecR7j0lK6Lf92E213a2x7g9sMX9J\n" \
"Qd8+S2WlsLAChOzsEe+xB5tycUoxAoHBANeGKQcWNmNtMXahRHpKlBxrqweVLDHh\n" \
"VT3H+8oOKRP5KgQ51GCAbOD4rWMDfliP1sysfDSDycpFfQoXu97R+uiWHVToSJEz\n" \
"9Ksc0IGl9qaKD4zSQWzqcX7l685DToMd97jnkmTQfQZSvN/7tHSzsduwjEM0Vb1I\n" \
"uTtncg/YVdzO82WyWH1Nt3xh70dO8EcifBAoaoOQ+4gcf6pWQC+NsZWDU2x1N3rl\n" \
"euS6ZlW0P/C1qeWxsV3FhHX5kLxke2fAYwKBwQDQF3tqsEWInnDa2fsDWq1etQdJ\n" \
"lxzfaRFmxi72fDz6Q5nuk5sM8Up4YGz8wbSL3hDjnvUjuqDh5p6c5MvnXO1wsniQ\n" \
"cVku8RYbc6Vobn70X6OG/apJBqzWfX3vWBZhp+SPmx+FwJ3ivBgwjM7evfgq2eNx\n" \
"J5h7REDi7GF3NuUfTu69I8L+WrWwbD4g/IS5g+XmYG9q5Hgu3XT4Q6NolnZPGzOC\n" \
"ZeERZbqq+rC5oAzDr2gPaGLHzmyuUjiYqfeVzckCgcB7OgL1QVcBjCou2NANV9zN\n" \
"TyYvpLPrJRoWw+QLyALmn5zNY78xGdym2KhSJ2oy+rLmvWwanoteslJm1eEd6lMa\n" \
"btIAfPsYwmsO9t+kcR7QY7m2W3T68CTKWG11tngKcbT0rLrAF5CUb/P74K9x1lhH\n" \
"8u6PcjnfitgVxfSqaJfXMyBpMgp0Ql45qMklrVWmZmom1uTnqpqWXS0FWGFEjzBM\n" \
"+xRUu8Rn6RNV+dmPAQtZ9pRYdzuhENDGn1WI0a7ZSF8CgcACm1izdHPai0mUpEht\n" \
"uVQUrKXjuR3mepV6OR9bfSDybahfAcBegCNTsLmMzQxq+CQ0BcjCYtMjIVV5V7Bq\n" \
"PJLI56N1A8Z/PQvu2hsHXOMAQrZ0uXbJpWjC4x2RwwO8w3Ksjd/uOASCU6Ll8M2i\n" \
"orSaLNKZl0pkuO0rKpTp/mkOZFm/StRlhVNjW2fH2AH9pPZbVXfv2d7bj3Cor4g7\n" \
"hi8DPK7/gjE4m5KkE7HlEWk3hloCX2dlY2OfJ26IExED8mkCgcB7PaPfHE9PPxIK\n" \
"5QZ6giYougWHh11m9q8Mwqhx3CDvoGEaYkKrHr/Bj4UQBbTk0Z7pmBpL4r5wfKfZ\n" \
"1Xh/TGazbwfUSwSQ0sH/0dd7UwzlAj3IGXGIQtbuNFT8Vyw8gQHZGEhVIXEVLZM8\n" \
"nz8fdU2BWs2JK5t7SGR9ueloeid5bGHE1UuzHH9XJXawZMPB0T63vXPNBlMxf7do\n" \
"yF5ABlh23I+V/DDC/6OhSjcG27A7OC6jWULthHNwRSFKezr7dlg=\n" \
"-----END RSA PRIVATE KEY-----\n\0";

const char* publicKey = "-----BEGIN RSA PUBLIC KEY-----\n" \
"MIIBigKCAYEArzDGSrlZWqmTFN+RKdZg3S/gHkdLKXny6TrZheEHUAxDlr8kNcbC\n" \
"zGXgV1WPCMFbBM9DSzdq6jqljXY6vEbFJ1gMN524Il6n0U0mZdKQBKFcIVFwJxXR\n" \
"ztQyNUjQO3hRRTDf+ZDlWDqy8KjXivVk9x4t3h5TVV52fOlQ1tWsv4HdgwRzumrH\n" \
"lpIk8pacvTyj71+NE17csy4IdQrdoIzYDtJU++8OHwkzO69mvFPkTag1ZjQXMebj\n" \
"9bDn3M/zeQfVFyadI2ZuN4/VwHJTQIPThv4f81HlsF+yn71+RSfOPgnQ8CraUG5h\n" \
"5FSZg096NwqmIGJPFBuCQDYYSJu8WhgZOlGTFzG9iqvm0JYKa0qQdUVr5CucDGMw\n" \
"XcCxCxhFIacwaQcxfAQCX6MBiMsolAqRb2re2erYFoavVo93y2UgIZA/5MXTSpZo\n" \
"unRiqN6LWtEgYHaLlA7dPmbRGoa+/iIRtS+E7LJHHEnLlgPJ+ym5vx4acmd6N98p\n" \
"s1BuGRR4JFS7AgMBAAE=\n" \
"-----END RSA PUBLIC KEY-----";

// const char* privateKey ="-----BEGIN RSA PRIVATE KEY-----\n"\
// "MIIEowIBAAKCAQEAy8Dbv8prpJ/0kKhlGeJYozo2t60EG8L0561g13R29LvMR5hy\n"\
// "vGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+vw1HocOAZtWK0z3r26uA8kQYOKX9\n"\
// "Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQApfc9jB9nTzphOgM4JiEYvlV8FLhg9\n"\
// "yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68i6T4nNq7NWC+UNVjQHxNQMQMzU6l\n"\
// "WCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoVPpY72+eVthKzpMeyHkBn7ciumk5q\n"\
// "gLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUywQIDAQABAoIBADhg1u1Mv1hAAlX8\n"\
// "omz1Gn2f4AAW2aos2cM5UDCNw1SYmj+9SRIkaxjRsE/C4o9sw1oxrg1/z6kajV0e\n"\
// "N/t008FdlVKHXAIYWF93JMoVvIpMmT8jft6AN/y3NMpivgt2inmmEJZYNioFJKZG\n"\
// "X+/vKYvsVISZm2fw8NfnKvAQK55yu+GRWBZGOeS9K+LbYvOwcrjKhHz66m4bedKd\n"\
// "gVAix6NE5iwmjNXktSQlJMCjbtdNXg/xo1/G4kG2p/MO1HLcKfe1N5FgBiXj3Qjl\n"\
// "vgvjJZkh1as2KTgaPOBqZaP03738VnYg23ISyvfT/teArVGtxrmFP7939EvJFKpF\n"\
// "1wTxuDkCgYEA7t0DR37zt+dEJy+5vm7zSmN97VenwQJFWMiulkHGa0yU3lLasxxu\n"\
// "m0oUtndIjenIvSx6t3Y+agK2F3EPbb0AZ5wZ1p1IXs4vktgeQwSSBdqcM8LZFDvZ\n"\
// "uPboQnJoRdIkd62XnP5ekIEIBAfOp8v2wFpSfE7nNH2u4CpAXNSF9HsCgYEA2l8D\n"\
// "JrDE5m9Kkn+J4l+AdGfeBL1igPF3DnuPoV67BpgiaAgI4h25UJzXiDKKoa706S0D\n"\
// "4XB74zOLX11MaGPMIdhlG+SgeQfNoC5lE4ZWXNyESJH1SVgRGT9nBC2vtL6bxCVV\n"\
// "WBkTeC5D6c/QXcai6yw6OYyNNdp0uznKURe1xvMCgYBVYYcEjWqMuAvyferFGV+5\n"\
// "nWqr5gM+yJMFM2bEqupD/HHSLoeiMm2O8KIKvwSeRYzNohKTdZ7FwgZYxr8fGMoG\n"\
// "PxQ1VK9DxCvZL4tRpVaU5Rmknud9hg9DQG6xIbgIDR+f79sb8QjYWmcFGc1SyWOA\n"\
// "SkjlykZ2yt4xnqi3BfiD9QKBgGqLgRYXmXp1QoVIBRaWUi55nzHg1XbkWZqPXvz1\n"\
// "I3uMLv1jLjJlHk3euKqTPmC05HoApKwSHeA0/gOBmg404xyAYJTDcCidTg6hlF96\n"\
// "ZBja3xApZuxqM62F6dV4FQqzFX0WWhWp5n301N33r0qR6FumMKJzmVJ1TA8tmzEF\n"\
// "yINRAoGBAJqioYs8rK6eXzA8ywYLjqTLu/yQSLBn/4ta36K8DyCoLNlNxSuox+A5\n"\
// "w6z2vEfRVQDq4Hm4vBzjdi3QfYLNkTiTqLcvgWZ+eX44ogXtdTDO7c+GeMKWz4XX\n"\
// "uJSUVL5+CVjKLjZEJ6Qc2WZLl94xSwL71E41H4YciVnSCQxVc4Jw\n"\
// "-----END RSA PRIVATE KEY-----\n\0";

// const char* publicKey ="-----BEGIN PUBLIC KEY-----\n"\
// "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAy8Dbv8prpJ/0kKhlGeJY\n"\
// "ozo2t60EG8L0561g13R29LvMR5hyvGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+\n"\
// "vw1HocOAZtWK0z3r26uA8kQYOKX9Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQAp\n"\
// "fc9jB9nTzphOgM4JiEYvlV8FLhg9yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68\n"\
// "i6T4nNq7NWC+UNVjQHxNQMQMzU6lWCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoV\n"\
// "PpY72+eVthKzpMeyHkBn7ciumk5qgLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUy\n"\
// "wQIDAQAB\n"\
// "-----END PUBLIC KEY-----\n";

RSA* createPrivateRSA(const char* key) {
  RSA *rsa = NULL;
  BIO * keybio = BIO_new_mem_buf((void*)key, -1);
  if (keybio==NULL) {
      return 0;
  }
  rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
  return rsa;
}

RSA* createPublicRSA(const char* key) {
  RSA *rsa = NULL;
  BIO *keybio;
  keybio = BIO_new_mem_buf((void*)key, -1);
  if (keybio==NULL) {
      return 0;
  }
  rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa,NULL, NULL);
  ERR_print_errors_fp(stderr);
	// printf("%s\n", errcode(szErrMsg));
  return rsa;
}

bool RSASign( RSA* rsa,
              const unsigned char* Msg,
              size_t MsgLen,
              unsigned char** EncMsg,
              size_t* MsgLenEnc) {
  EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
  EVP_PKEY* priKey  = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(priKey, rsa);
  if (EVP_DigestSignInit(m_RSASignCtx,NULL, EVP_sha256(), NULL,priKey)<=0) {
      return false;
  }
  if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0) {
      return false;
  }
  if (EVP_DigestSignFinal(m_RSASignCtx, NULL, MsgLenEnc) <=0) {
      return false;
  }
  *EncMsg = (unsigned char*)malloc(*MsgLenEnc);
  if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0) {
      return false;
  }
  EVP_MD_CTX_free(m_RSASignCtx);
  return true;
}

bool RSAVerifySignature( RSA* rsa,
                         unsigned char* MsgHash,
                         size_t MsgHashLen,
                         const char* Msg,
                         size_t MsgLen,
                         bool* Authentic) {
  *Authentic = false;
  EVP_PKEY* pubKey  = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(pubKey, rsa);
  EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_create();

  if (EVP_DigestVerifyInit(m_RSAVerifyCtx,NULL, EVP_sha256(),NULL,pubKey)<=0) {
    return false;
  }
  if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, Msg, MsgLen) <= 0) {
    return false;
  }
  int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, MsgHash, MsgHashLen);
  if (AuthStatus==1) {
    *Authentic = true;
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return true;
  } else if(AuthStatus==0){
    *Authentic = false;
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return true;
  } else{
    *Authentic = false;
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return false;
  }
}

void Base64Encode( const unsigned char* buffer,
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

void Base64Decode(const char* b64message, unsigned char** buffer, size_t* length) {
  BIO *bio, *b64;

  int decodeLen = calcDecodeLength(b64message);
  *buffer = (unsigned char*)malloc(decodeLen + 1);
  (*buffer)[decodeLen] = '\0';

  bio = BIO_new_mem_buf(b64message, -1);
  b64 = BIO_new(BIO_f_base64());
  bio = BIO_push(b64, bio);

  *length = BIO_read(bio, *buffer, strlen(b64message));
  BIO_free_all(bio);
}

char* signMessage(const char* privateKey, const char* plainText) {
  RSA* privateRSA = createPrivateRSA(privateKey); 
  unsigned char* encMessage;
  char* base64Text;
  size_t encMessageLength;
  RSASign(privateRSA, (unsigned char*) plainText, strlen(plainText), &encMessage, &encMessageLength);
  Base64Encode(encMessage, encMessageLength, &base64Text);
  free(encMessage);
  return base64Text;
}

bool verifySignature(const char* publicKey, const char* plainText, char* signatureBase64) {
  RSA* publicRSA = createPublicRSA(publicKey);
  unsigned char* encMessage;
  size_t encMessageLength;
  bool authentic;
  Base64Decode(signatureBase64, &encMessage, &encMessageLength);
  bool result = RSAVerifySignature(publicRSA, encMessage, encMessageLength, plainText, strlen(plainText), &authentic);
  //std::cout << 
  return result & authentic;
}

int main() {
  const char* plainText = "My secret message.\n";
  char* signature = signMessage(privateKey, plainText);
  bool authentic = verifySignature(publicKey, "My secret message.\n", signature);
  if ( authentic ) {
    std::cout << "Authentic" << std::endl;
  } else {
    std::cout << "Not Authentic" << std::endl;
  }
}
