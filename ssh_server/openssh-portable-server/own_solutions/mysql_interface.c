#include "mysql_interface.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

//enum MYSQL_STATE
//{
//    DISCONNECTED,
//    CONNECTED
//} mysql_state = DISCONNECTED;

//MYSQL mysql;

//void connect_db()
//{
//    mysql_init(&mysql);
//    if (!mysql_real_connect(&mysql, "172.16.16.3", "root", "pass",
//                               "ssh", 0, NULL, CLIENT_FOUND_ROWS))
//    {
//        debug("Something went wrong connecting to db!");
//        return;
//    }
//    mysql_state = CONNECTED;
//}

void finish_with_error(MYSQL *con)
{
    debug(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void insert_log(struct LOG_DATA log)
{
    //if (log == NULL)
    //{
    //    debug("mysql_interface.c: Passed log pointer is NULL!");
    //    return;
    //}

    //if (mysql_state == DISCONNECTED)
    //{
    //    connect_db();        
    //}
    //if (!mysql_real_connect(&mysql, "172.16.16.3", "root", "pass",
    //                           "ssh", 0, NULL, NULL))
    //{
    //    debug("%s", "Something went wrong connecting to db!");
    //    return;
    //}

    const char *q1 = "INSERT INTO ssh_log (user, from_ip, from_port) VALUES (";

    const char *q2 = ")";
    char from_port[6];

    sprintf(from_port, "%d", log.from_port);
                     
    int query_size = 1024; //strlen(q1) +
                     //strlen(log.user) + 
                     //1 + 
                     //1 +
                     //strlen(log.from_ip) + 
                     //5 +
                     //strlen(q2) + 
                     //10;
    
    char *query_str = malloc(query_size);
    strcpy(query_str, q1);
    
    strcat(query_str, " '");
    strcat(query_str, log.user);
    strcat(query_str, "', ");


    strcat(query_str, " '");
    strcat(query_str, log.from_ip);
    strcat(query_str, "', ");

    strcat(query_str, from_port);

    strcat(query_str, q2);
    debug("%s", query_str);

    MYSQL *mysql = mysql_init(NULL);
    if (!mysql_real_connect(mysql, "172.16.16.3", "root", "pass",
                               "ssh", 0, NULL, CLIENT_FOUND_ROWS))
    {
        debug("%s", "Something went wrong connecting to db!");
        return;
    }
    if (mysql_query(mysql, query_str)) {
        finish_with_error(mysql);
    }

    mysql_close(mysql);
    free(query_str);

}


int get_key(char *pub_key)
{
    MYSQL *mysql = mysql_init(NULL);
    //mysql_init(&mysql);
    if (!mysql_real_connect(mysql, "172.16.16.3", "root", "pass",
                               "ssh", 0, NULL, CLIENT_FOUND_ROWS))
    {
        debug("%s", "Something went wrong connecting to db!");
        return 0;
    }
    //if (mysql_state == DISCONNECTED)
    //{
    //    connect_db();
    //}

    MYSQL_RES *query_id;
    unsigned long current_timestamp_i = (unsigned long)time(NULL);
    char current_timestamp_c[15];
    sprintf(current_timestamp_c, "%d", current_timestamp_i);

    const char *q1 = "SELECT id FROM ssh_keys WHERE (pub_key_converted='";
    const char *q2 = "' AND (valid_through = 0 OR valid_through > ";
    const char *q3 = "))";

    char *query_str = malloc(strlen(q1) + strlen(pub_key) + strlen(q2) + strlen(current_timestamp_c) + 1);
    int position = 0;
    memcpy(query_str, q1, strlen(q1)); //without \0
    position += strlen(q1);
    memcpy(query_str + position, pub_key, strlen(pub_key));
    position += strlen(pub_key);
    memcpy(query_str + position, q2, strlen(q2));
    position += strlen(q2);
    memcpy(query_str + position, current_timestamp_c, strlen(current_timestamp_c));
    position += strlen(current_timestamp_c);
    memcpy(query_str + position, q3, strlen(q3)+1);
    position += strlen(q3);
    
    //debug("querry: %s", query_str);

    mysql_query(mysql, query_str);
    query_id = mysql_store_result(mysql);
    int    row_count = query_id->row_count;

    debug("number of rows %lu", row_count );

    mysql_close(mysql);
    free(query_str);
    return row_count;
}

