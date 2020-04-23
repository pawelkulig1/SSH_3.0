#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */

char *send_msg(const char *message, size_t *size);
void get_server_pub_key_tofu();
int request_public_key_check(const char **public_key);
const char *send_updated_key(const char **signed_key, const char **public_key, const char **new_public_key);
void encrypt_message(const char *msg_in, char* key, char* iv, char *msg_out); 

#endif // HTTP_REQUEST_H