#include <iostream>
#include <mysql/mysql.h>

extern "C" {

void testowa()
{
	std::cout<<"connect start!" << std::endl;
	MYSQL mysql;
	std::cout<<"obj created" << std::endl;
	std::cout<<&mysql<<std::endl;
	
	std::cout<<mysql_init(nullptr) << std::endl;
	mysql_init(&mysql);
	std::cout<<"init worked!" << std::endl;
	if(!mysql_real_connect(&mysql, "172.16.16.3", "root", "pass", 0, 0, nullptr, 0))
	{
		std::cout<<"not working"<<std::endl;
	}	
	mysql_close(&mysql);
	std::cout << "test123" << std::endl;
}

} //extern "C"
