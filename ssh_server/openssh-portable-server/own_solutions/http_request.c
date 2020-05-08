
#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
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
    unsigned char encrypted[MSG_SIZE];
    memset(encrypted, 0, MSG_SIZE);
    size_t size_encrypted = symmetric_encrypt(msg_in, key, iv, encrypted);
    encrypted[size_encrypted] = 0;
    
    BUF_MEM *ptr1 = NULL;
    BUF_MEM *ptr2 = NULL;
    BUF_MEM *ptr3 = NULL;

    unsigned char* b64_encrypted = NULL;
    Base64Encode(encrypted, size_encrypted, &b64_encrypted, &ptr1);
    char b64_enc[strlen(b64_encrypted) + 1];

    size_t size = strlen(b64_encrypted);
    memcpy(b64_enc, b64_encrypted, size);
    b64_enc[size] = 0;

    //asymetric enc of key
    unsigned char enc_key[129];
    size = encrypt(key, enc_key);
    enc_key[128] = 0;

    unsigned char *b64_key = NULL;
    Base64Encode(enc_key, size, &b64_key, &ptr2);
    size = strlen(b64_key);
    char b64_k[size + 1];
    memcpy(b64_k, b64_key, size);
    b64_k[size] = 0;

    unsigned char *b64_iv = NULL;
    Base64Encode(iv, 16, &b64_iv, &ptr3);
    size = strlen(b64_iv);
    char b64_i[size + 1];
    memcpy(b64_i, b64_iv, size);
    b64_i[size] = 0;
    
    //key|iv|msg
    char *fmt2 = "%s|%s|%s;";
    
    sprintf(msg_out, fmt2, b64_k, b64_i, b64_enc);
    BUF_MEM_free(ptr1);
    BUF_MEM_free(ptr2);
    BUF_MEM_free(ptr3);
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

void encode_send_recv_decode(char *in, char *out)
{
    unsigned char key[33];// = malloc(33);
    unsigned char iv[17]; // = malloc(17);
    get_random(key, 32);
    get_random(iv, 16);
    // memcpy(key, "aaaabbbbaaaabbbbaaaabbbbaaaabbbb", 32);
    // memcpy(iv, "aaaabbbbaaaabbbb", 16);
    key[32] = 0;
    iv[16] = 0;

    char encrypted_message[MSG_SIZE]; // = malloc(MSG_SIZE);
    // memset(encrypted_message, '0', MSG_SIZE);
    encrypt_message(in, key, iv, encrypted_message); 

    size_t recv_size;
    char b64_encrypted_response[MSG_SIZE];
    send_msg(encrypted_message, &recv_size, b64_encrypted_response);
    // printf("%d, %s\n", recv_size, b64_encrypted_response);
    b64_encrypted_response[recv_size] = 0;
    
    size_t response_size;
    unsigned char *encrypted_response = NULL;
    Base64Decode(b64_encrypted_response, &encrypted_response, &response_size);
    
    char encrypted_resp[MSG_SIZE];
    memcpy(encrypted_resp, encrypted_response, response_size);
    free(encrypted_response);

    //char response[MSG_SIZE]; //= malloc(MSG_SIZE);
    // memset(response, 0, MSG_SIZE);
    symmetric_decrypt(encrypted_resp, recv_size, key, iv, out);
    out[recv_size] = 0;

}

int request_public_key_check(const char **public_key)
{
    const max_message_size = MSG_SIZE;
    unsigned char message[MSG_SIZE];// = malloc(max_message_size);
    // memset(message, 0, max_message_size);
    unsigned const char *message_fmt = "opcode=%c|public_key=%s|rnd=%s;";
    unsigned char opcode = '0';
    unsigned char random_chars[255];// = malloc(256);
    get_random(random_chars, 255);
    random_chars[255] = 0;

    sprintf(message, message_fmt, opcode, *public_key, random_chars);    
    char response[MSG_SIZE];
    encode_send_recv_decode(message, response);

    // printf("%s", response);
    int pos = find_pos(response, '|');
    if (pos != -1)
    {
        int key_allowed = response[pos + 1] - '0';
        // printf("key_allowed: %d", key_allowed);
        if (key_allowed > 0)
        {
            int proper_key = strcmp(response + pos + 2, random_chars);
            if (proper_key == 0)
            {
                return key_allowed;
            }
        }
        return 0;
    }
    return 0;
}

char *send_msg(const char *message, size_t* size, char *out)
{
    const char *host = "172.18.0.4"; //TODO
    const int port = 5001; //TODO
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    size_t response_size = MSG_SIZE;
    char response[response_size]; //malloc(response_size);

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
    memset(response, 0, MSG_SIZE);
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
    // unsigned char *real_resp = malloc(received + 1);
    memcpy(out, response, received);
    out[received] = 0;
    //free(response);
    /* close the socket */
    close(sockfd);
    // return real_resp;

}

const char *send_updated_key(const char **signed_key, const char **public_key, const char **new_public_key)
{
    //TODO if cannot connect don't update key
    const char *message_fmt = "opcode=%c|signed_key=%s|old_pub_key=%s|new_public_key=%s;";
    const char opcode = '1';
    char message[MSG_SIZE];
    sprintf(message, message_fmt, opcode, *signed_key, *public_key, *new_public_key);

    char response[MSG_SIZE];
    encode_send_recv_decode(message, response);
}

int save_log(const char **log) 
{
    const char *message_fmt = "opcode=%c|log=%s;";
    const char opcode = '2';
    char message[MSG_SIZE];
    sprintf(message, message_fmt, opcode, *log);

    char response[MSG_SIZE];
    encode_send_recv_decode(message, response);
}

