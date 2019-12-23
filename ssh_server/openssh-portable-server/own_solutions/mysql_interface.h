#ifndef MYSQL_INTERFACE_H
#define MYSQL_INTERFACE_H
#include <mysql/mysql.h>

#define ROW_MAX_LEN 1024

enum KeyStatus 
{
	NOT_AVAILABLE = 0,
	AVAILABLE,
	NEEDS_RENEWAL
};


struct LOG_DATA
{
    const char *user;
    const char *from_ip;
    int from_port;
};


void finish_with_error(MYSQL *con);

enum KeyStatus get_key(char *pub_key);
void insert_log(struct LOG_DATA log);

//void connect_db();

#endif //MYSQL_INTERFACE_H
