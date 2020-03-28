mysql_test:
	g++ mysql/mysql_test.cpp -lmysqlclient -o mysql/mysql_test.o

convert_key:
	g++ -fPIC -shared convert_key.cpp b64_pton.cpp -lssl -lcrypto -o convert_key.so
