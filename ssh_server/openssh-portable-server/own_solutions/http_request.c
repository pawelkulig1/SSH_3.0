
#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <errno.h>
#include <openssl/rand.h>
#include <string.h>
#include "encrypt.h"
#include "http_request.h"
#include <sodium.h>

void get_random(char *out, int len) 
{
    int sodium_status = 1;
    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized, it is not safe to use */
        printf("Sodium could not be initialized");
        sodium_status = 0;
    }
    else
    {
        const char* possible = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
        size_t max_val = strlen(possible);
        for (int i=0;i<len;i++)
        {
            int random_number;
            if (sodium_status)
            {
                random_number = randombytes_uniform(max_val);
            }
            else
            {
                random_number = rand() % max_val;
            }
            
            out[i] = possible[random_number];
        }
    }
}


// void get_server_pub_key_tofu()
// {
//     char message[8];
//     const char *message_fmt = "opcode=%c|random=%s;";
//     const char opcode = '2';
//     char random[256];
//     RAND_bytes(random, 255);
//     sprintf(message, message_fmt, opcode, random);
//     send_msg(message);
// }

void encrypt_message(const char *msg_in, char* key, char* iv, char *msg_out)
{  
    unsigned char *encrypted = malloc(2048);
    size_t size_encrypted = symmetric_encrypt(msg_in, key, iv, encrypted); 
    unsigned char* b64_encrypted = NULL;
    
    Base64Encode(encrypted, size_encrypted, &b64_encrypted);
    free(encrypted);

    //asymetric enc of key
    unsigned char enc_key[128];
    memset(enc_key, 0, 128);

    size_t size = encrypt(key, enc_key);
    unsigned char *b64_key = NULL;
    unsigned char *b64_iv = NULL;
    Base64Encode(enc_key, size, &b64_key);
    Base64Encode(iv, 16, &b64_iv);
    
    //key|iv|msg
    char *fmt2 = "%s|%s|%s;";
    memset(msg_out, 0, 16384);
    
    sprintf(msg_out, fmt2, b64_key, b64_iv, b64_encrypted);
    
}

int find_pos(const char *cstr, const char c)
{
    char *pos = cstr;
    int counter = 0;
    while (c != *pos)
    {
        
        if (*pos == 0) return -1;
        ++pos;
        ++counter;
    }
    return counter;
}

int request_public_key_check(const char **public_key)
{
    unsigned char *message = malloc(4096);
    memset(message, 0, 16384);
    unsigned char *message_fmt = "opcode=%c|public_key=%s|rnd=%s;";
    unsigned char opcode = '0';
    unsigned char *random_chars = malloc(256);
    get_random(random_chars, 255);
    random_chars[255] = 0;

    sprintf(message, message_fmt, opcode, *public_key, random_chars);    
    
    unsigned char key[33];
    unsigned char iv[17];
    get_random(key, 32);
    get_random(iv, 16);
    key[32] = 0;
    iv[16] = 0;

    const char *encrypted_message = malloc(4096);
    encrypt_message(message, key, iv, encrypted_message); 

    size_t recv_size;
    char *b64_encrypted_response = send_msg(encrypted_message, &recv_size);
    // printf("%d, %s\n", recv_size, b64_encrypted_response);
    
    size_t response_size;
    unsigned char *encrypted_response;
    Base64Decode(b64_encrypted_response, recv_size, &encrypted_response, &response_size);
    char *response = malloc(2048);
    memset(response, 0, 2048);
    symmetric_decrypt(encrypted_response, recv_size, key, iv, response);

    printf("%s", response);
    int pos = find_pos(response, '|');
    if (pos != -1)
    {
        int key_allowed = response[pos + 1] - '0';
        printf("key_allowed: %d", key_allowed);
        if (key_allowed)
        {
            int proper_key = strcmp(response + pos + 2, random_chars);
            if (proper_key == 0)
            {
                return 1;
            }
        }
        return 0;
    }
    return 0;

    free(message);
    free(random_chars);
    free(encrypted_message);
    free(b64_encrypted_response);
    free(encrypted_response);
    free(response);
}

char *send_msg(const char *message, size_t* size)
{
    const char *host = "172.18.0.7"; //TODO
    const int port = 5006; //TODO
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    size_t response_size = 4096;
    char *response = malloc(response_size);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR connecting to Globa Key Storage! %d\n", errno);
        exit(errno);
    }
    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0){
            //error("ERROR writing message to socket");
            printf("ERROR writing message to Global Key Storage!\n");
            exit(1);
            break;
        }
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    memset(response, 0, sizeof(response));
    total = response_size-1;
    received = 0;
    do {
        bytes = read(sockfd, response + received, total - received);
        if (bytes < 0) {
            error("ERROR reading response from socket");
            break;
        }
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket");

    *size = received;
    /* close the socket */
    close(sockfd);
    return response;

}

const char *send_updated_key(const char **signed_key, const char **public_key, const char **new_public_key)
{
    //TODO if cannot connect don't update key
    const char *message_fmt = "opcode=%c|signed_key=%s|old_pub_key=%s|new_public_key=%s;";
    const char opcode = '1';
    char *message = malloc(16384);
    sprintf(message, message_fmt, opcode, *signed_key, *public_key, *new_public_key);
    size_t recv_size;
    const char* out = send_msg(message, &recv_size);
    free(message);
    return out;
}

