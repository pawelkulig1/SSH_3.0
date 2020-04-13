#include "mysql_interface.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static const char *SERVER_IP = "172.18.0.2";


void finish_with_error(MYSQL *con)
{
    debug("MySQL error: %s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void insert_log(struct LOG_DATA log)
{

    const char *q1 = "INSERT INTO ssh_log (user, from_ip, from_port) VALUES (";

    const char *q2 = ")";
    char from_port[6];

    sprintf(from_port, "%d", log.from_port);
                     
    int query_size = 1024;
    
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
    if (!mysql_real_connect(mysql, SERVER_IP, "root", "pass",
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

enum KeyStatus get_key(char *pub_key, int *id)
{
    *id = -1;
    MYSQL *mysql = mysql_init(NULL);
    if (!mysql_real_connect(mysql, SERVER_IP, "root", "pass",
                               "ssh", 0, NULL, CLIENT_FOUND_ROWS))
    {
        debug("%s", "Something went wrong connecting to db!");
        return 0;
    }

    MYSQL_RES *result;
    unsigned long current_timestamp_i = (unsigned long)time(NULL);
    char current_timestamp_c[15];
    sprintf(current_timestamp_c, "%d", current_timestamp_i);

    const char *q1 = "SELECT id, UNIX_TIMESTAMP(valid_through), UNIX_TIMESTAMP(renewable_by) FROM ssh_keys WHERE (pub_key_converted='";
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
	enum KeyStatus status = NOT_AVAILABLE;
	MYSQL_ROW row;
	if (row_count > 0 && (row = mysql_fetch_row(result)))
	{
        *id = atoi(row[0]);
		int valid_through = atoi(row[1]);
		int renewal_time = atoi(row[2]);

		int valid = 0;
		int renew = 0;
		if (valid_through > current_timestamp_i || valid_through == 0) valid = 1;
		if (renewal_time > current_timestamp_i || renewal_time == 0) renew = 1;
        debug("valid: %u, renew: %u, valid_through: %u, renewable_by: %u", valid, renew, valid_through, renewal_time);


		//everything correct
		if (valid && renew)
		{
			status = AVAILABLE;
			// break;
		}
        else if (!valid) //key not usable - disconnect
		{
			status = NOT_AVAILABLE;
			// break;
		}
		else if (valid && !renew) //key needs to be renewed but only if is still valid
		{
			status = NEEDS_RENEWAL;
		}
	}

    mysql_close(mysql);
    free(query_str);
	return status;
}

void get_config(MYSQL *mysql, int *validity_period, int *renewability_period, int id)
{
    const char *query1 = "SELECT validity_period, renewability_period from ssh_config;";
    MYSQL_RES *result;
    if (mysql_query(mysql, query1))
    {
        finish_with_error(mysql);
    }

    result = mysql_store_result(mysql);
    MYSQL_ROW row;
    while (row = mysql_fetch_row(result))
    {
        *validity_period = atoi(row[0]);
		*renewability_period = atoi(row[0]);
        debug("dupa");
    }

}

int change_key(const u_char *new_key, const u_char *new_key_converted, int id)
{
    unsigned long current_timestamp_i = (unsigned long)time(NULL);
    // char current_timestamp_c[15];
    // sprintf(current_timestamp_c, "%d", current_timestamp_i);
    
    MYSQL *mysql = mysql_init(NULL);
    if (!mysql_real_connect(mysql, SERVER_IP, "root", "pass",
                               "ssh", 0, NULL, CLIENT_FOUND_ROWS))
    {
        debug("%s", "Something went wrong connecting to db!");
        return 0;
    }

    debug("%s", new_key);
    int validity_period = 0;
    int renewability_period = 0;
    get_config(mysql, &validity_period, &renewability_period, id);
    debug("change_key: %d, %d", validity_period, renewability_period);
    
    int renewable_by = 0;
    int valid_through = 0;

    const char sub_q1[50] = "renewable_by=0, ";
    if (renewability_period != 0)
    {
        renewable_by = current_timestamp_i + renewability_period;
        sprintf(sub_q1, "renewable_by=FROM_UNIXTIME(%d), ", renewable_by);
    }

    const char sub_q2[50] = "valid_through=0 ";
    if (validity_period != 0)
    {
        valid_through = current_timestamp_i + validity_period;
        sprintf(sub_q2, "valid_through=FROM_UNIXTIME(%d)", valid_through);
    }
    
    const char sub_q3[50];
    sprintf(sub_q3, "WHERE id=%d;", id);

    const char *query = malloc(8192);
    memset(query, '0', 8192);
    sprintf(query, "UPDATE ssh_keys SET "
        "pub_key='%s', "
        "pub_key_converted='%s', "
        "last_renewal=FROM_UNIXTIME(%d), ",
        "", new_key_converted, current_timestamp_i);

    strcat(query, sub_q1);
    strcat(query, sub_q2);
    strcat(query, sub_q3);

    int error;
    if ((error = mysql_query(mysql, query)))
    {
        debug("finishing with error %d", error);
        free(query);
        finish_with_error(mysql);
    }
    free(query);
    mysql_close(mysql);
}