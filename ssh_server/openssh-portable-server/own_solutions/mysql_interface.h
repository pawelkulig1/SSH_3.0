#ifndef MYSQL_INTERFACE_H
#define MYSQL_INTERFACE_H
#include <mysql/mysql.h>

#define ROW_MAX_LEN 4096

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

enum KeyStatus get_key(char *pub_key, int *id);

void insert_log(struct LOG_DATA log);

/*find next validity date and renewability date*/
void get_config(MYSQL *mysql, int *validity_period, int *renewability_period, int id);

//update key in database if recalculation happened
int change_key(const u_char *new_key, const u_char *new_key_converted, int id);

int get_empty_key();

//struct sshkey *string_to_key(char *str);

//void connect_db();

#endif //MYSQL_INTERFACE_H
