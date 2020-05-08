all:
	g++ -fPIC -shared sign.c -lssl -lcrypto -o sign.so
	g++ -fPIC -shared verify.c -lssl -lcrypto -o verify.so