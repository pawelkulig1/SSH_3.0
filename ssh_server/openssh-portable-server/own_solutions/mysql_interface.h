#ifndef MYSQL_INTERFACE_H
#define MYSQL_INTERFACE_H
#include <mysql/mysql.h>

#define ROW_MAX_LEN 1024
#define MAX_NUM_OF_ROWS 1000

char *get_all_keys(int *counter);

#endif //MYSQL_INTERFACE_H
