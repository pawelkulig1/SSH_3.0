#ifndef MYSQL_INTERFACE_H
#define MYSQL_INTERFACE_H
#include <mysql/mysql.h>

#define ROW_MAX_LEN 1024
//#define MAX_NUM_OF_ROWS 1000

int get_key(char *pub_key);
void connect_db();

#endif //MYSQL_INTERFACE_H
