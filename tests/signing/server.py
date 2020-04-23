#!/usr/bin/env python

# import socket


# TCP_IP = '0.0.0.0'
# TCP_PORT = 5005
# BUFFER_SIZE = 1024  # Normally 1024, but we want fast response

# s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# s.bind((TCP_IP, TCP_PORT))
# s.listen(1)

# conn, addr = s.accept()
# print('Connection address:', addr)

# def analyze_data(data):
#     signed_data, pub_key = data.split("|||||")
#     print(signed_data, pub_key)


# while True:
#     all_data = ""
#     while 1:
#         data = conn.recv(BUFFER_SIZE)
#         all_data += data.decode()
#         print(all_data)
#         if all_data.find(";;;;;") != -1:
#             analyze_data(all_data.split(";;;;;")[0])
#             conn.send("ok".encode())  # echo


#         if not data: break
                
        
        
#         # print(all_data)
#     conn.close()

import socketserver
from threading import Thread
import mysql.connector

class MyTCPHandler(socketserver.BaseRequestHandler):
    """
    The RequestHandler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """
    def _connect_db(self):
        self.cnx = mysql.connector.connect(user='root', password='pass', host="db", database="ssh")
        self.cursor = self.cnx.cursor()

    def parse_message(self, message):
        signed_data, pub_key = message.split("|||||")
        signed_data = signed_data[11:]
        pub_key = pub_key[12:].replace("+", " ")
        print("pub_key:", pub_key)

        self._connect_db()
        query = "SELECT * FROM ssh_keys WHERE pub_key='{}';".format(pub_key)
        print("query:", query)
        self.cursor.execute(query)
        # data = []
        for data_tuple in self.cursor:
            print("data_tuple:", data_tuple)
            #data.append(list(data_tuple))
        self.cursor.close()
        self.cnx.close
        # return data

    
    def handle(self):
        # if self.all_data 
        # self.request is the TCP socket connected to the client
        self.data = self.request.recv(8192)
        # self.all_data += self.data
        decoded_data = self.data.decode()
        if decoded_data.find(";;;;;") != -1:
            #t = Thread(target=self.parse_message, args=[decoded_data.split(";;;;;")[0]])
            #.start()
            self.parse_message(decoded_data.split(";;;;;")[0])

        #print("{} wrote:".format(self.client_address[0]))
        #print(self.data)
        # just send back the same data, but upper-cased
        self.request.sendall(self.data.upper())

if __name__ == "__main__":
    HOST, PORT = "0.0.0.0", 5006

    # Create the server, binding to localhost on port 9999
    server = socketserver.TCPServer((HOST, PORT), MyTCPHandler)

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()