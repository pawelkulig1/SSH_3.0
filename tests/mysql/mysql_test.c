#include <mysql/mysql.h>
#include <stdio.h>

int main() 
{
	MYSQL mysql;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, "172.16.16.3", "root", "pass",
                                                    "ssh", 0, NULL, 0))
    {
		//std::cout<<"something went wrong connecting to db" << std::endl;
		printf("Something went wrong!");
		return 0;
    }

    MYSQL_RES *query_id;
    MYSQL_ROW row;

    mysql_query(&mysql, "SELECT pub_key from ssh_keys");
    query_id = mysql_store_result(&mysql);

    int counter = 0;
    while ((row = mysql_fetch_row(query_id)) != 0) 
    {
		printf("%s", row[0]);
		//std::cout << row[0] << std::endl;
    }
    mysql_close(&mysql);
}
