#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>

const char *
do_convert_to_pem(const char *rsa_key)
{
	// switch (sshkey_type_plain(k->type)) {
	// case KEY_RSA:
	// {
    const char *buffer = malloc(8192 * sizeof(char));
    bzero(buffer, 8192);
    FILE * PEM_key_file;
    PEM_key_file = tmpfile ();
    
    if (!PEM_write_RSAPublicKey(PEM_key_file, k->rsa))
        fatal("PEM_write_RSAPublicKey failed");
    
    rewind(PEM_key_file);

    const char* line[1024];
    while (!feof(PEM_key_file)) {
        if (fgets (line, 1024, PEM_key_file) == NULL) break;
        strcat(buffer, line);
    }
    return buffer;
}