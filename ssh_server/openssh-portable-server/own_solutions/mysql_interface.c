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

enum KeyStatus get_key(char *pub_key)
{
    MYSQL *mysql = mysql_init(NULL);
    if (!mysql_real_connect(mysql, "172.16.16.3", "root", "pass",
                               "ssh", 0, NULL, CLIENT_FOUND_ROWS))
    {
        debug("%s", "Something went wrong connecting to db!");
        return 0;
    }

    MYSQL_RES *result;
    unsigned long current_timestamp_i = (unsigned long)time(NULL);
    char current_timestamp_c[15];
    sprintf(current_timestamp_c, "%d", current_timestamp_i);

    const char *q1 = "SELECT valid_through, renewable_by FROM ssh_keys WHERE (pub_key_converted='";
    const char *q2 = "')";

	char *query_str = malloc(1024);
	strcpy(query_str, q1);
	strcat(query_str, pub_key);
	strcat(query_str, q2);
   	 
    debug("querry: %s", query_str);

    mysql_query(mysql, query_str);
    result = mysql_store_result(mysql);
    int row_count = mysql_num_rows(result);

    debug("number of rows %lu", row_count );
	enum KeyStatus status;
	MYSQL_ROW row;
	while (row = mysql_fetch_row(result))
	{
		int valid_through = atoi(row[0]);
		int renewal_time = atoi(row[0]);

		int valid = 0;
		int renew = 0;
		if (valid_through > current_timestamp_i || valid_through == 0) valid = 1;
		if (renewal_time > current_timestamp_i || renewal_time == 0) renew = 1;


		//everything correct
		if (valid && renew)
		{
			status = AVAILABLE;
			break;
		}

		//key not usable - disconnect
		if (!valid)
		{
			status = NOT_AVAILABLE;
			break;
		}

		//key needs to be renewed but only if is still valid
		if (valid && !renew)
		{
			status = NEEDS_RENEWAL;
		}

	}

    mysql_close(mysql);
    free(query_str);
	return status;
}

