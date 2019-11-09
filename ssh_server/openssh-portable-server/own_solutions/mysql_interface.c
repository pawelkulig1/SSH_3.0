#include "mysql_interface.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

char *get_all_keys(int *counter)
{
	*counter = 0;
	char *ret = malloc(ROW_MAX_LEN * MAX_NUM_OF_ROWS);
	if (ret == NULL)
	{
		printf("Something went wrong during memory allocation!");
		return NULL;
	}
	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, "172.16.16.3", "root", "pass",
			   				"ssh", 0, NULL, 0))
	{
		printf("Something went wrong connecting to db!");
		return NULL;
	}

	MYSQL_RES *query_id;
	MYSQL_ROW row;

	mysql_query(&mysql, "SELECT pub_key_converted from ssh_keys");
	query_id = mysql_store_result(&mysql);

	while ((row = mysql_fetch_row(query_id)) != 0)
	{
		long unsigned int length = *mysql_fetch_lengths(query_id);
		size_t size = length < ROW_MAX_LEN - 1 ? length : ROW_MAX_LEN - 1;
		memcpy(ret + (*counter) * ROW_MAX_LEN, row[0], size);
		ret[size + (*counter) * ROW_MAX_LEN] = '\n';

		(*counter)++;
		if (*counter >= MAX_NUM_OF_ROWS)
			break;
	}
	mysql_close(&mysql);
	return ret;
}

