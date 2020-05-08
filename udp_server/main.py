#!/usr/bin/env python

import socketserver
from threading import Thread
import mysql.connector

from convert_RSA_to_PEM import convert_RSA_to_PEM
from verify_interface import verify_signature
from sign_interface import sign
from common.db import DBConnector
import base64
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
import signal
from time import sleep


class MyTCPHandler(socketserver.BaseRequestHandler):
    """
    The RequestHandler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def parse_message(self, message):
        #key|iv|message
        enc_key, iv, message = message.split(b'|')
        
        enc_key = base64.b64decode(enc_key)
        iv = base64.b64decode(iv)
        message = base64.b64decode(message)
        
        f = open("rsa.private", "r")
        key = RSA.importKey(f.read())
        sym_key_decrypted = key.decrypt(enc_key)
        sym_key_decrypted = sym_key_decrypted[sym_key_decrypted.rfind(b'\0') + 1:]
        iv = iv.decode()
        sym_key = sym_key_decrypted.decode()

        cipher = AES.new(sym_key, AES.MODE_CBC, iv)
        decrypted_message = cipher.decrypt(message)
        message = decrypted_message.decode()
        
        dbConnector = DBConnector()
        message_source = str(self.client_address[0])
        if message.find(";") == -1:
            dbConnector.insert_log(message_source, "Bad message format: " + message)
            return
        
        message = message.split(";")[0]

        if message.find("opcode=") == -1:
            #bad message log and ignore
            dbConnector.insert_log(message_source, "Opcode not found in message: " + message)
            return
        
        opcode = int(message[7])
        
        if opcode == 0:
            #key verification
            _, pub_key, random_chars = message.split("|")
            pub_key = pub_key[11:]
            random_chars = random_chars[4:]
            found = dbConnector.is_key_in_base(pub_key)
            if found == 0:
                dbConnector.insert_log(message_source, "Public Key authentication failed")

            #add padding if required
            msg = str(found) + random_chars
            msg = str(len(msg)) + "|" + msg
            rest = len(msg) % 16
            for i in range(16 - rest):
                msg += "\0"

            cipher = AES.new(sym_key, AES.MODE_CBC, iv)
            out_enc = cipher.encrypt(msg)

            response = base64.b64encode(out_enc)

        elif opcode == 1:
            #key rotation
            response = "Failed".encode()
            
            _, signed_data, pub_key, new_pub_key = message.split("|")
            signed_data = signed_data[11:]
            pub_key = pub_key[12:]
            new_pub_key = new_pub_key[15:]
            signature_verified = None
            try:
                signature_verified = verify_signature(convert_RSA_to_PEM(pub_key), new_pub_key, signed_data)
            except:
                response = "Failed".encode()
                dbConnector.insert_log(message_source, "Signature verification failed during key rotation")
            if signature_verified == True:
                try:
                    dbConnector.rotate_public_key(pub_key, new_pub_key)
                    dbConnector.insert_log(message_source, "Key rotation performed successfuly")
                    response = "Success".encode()
                except:
                    dbConnector.insert_log(message_source, "Exception occured during key rotation")
                    response = "Failed".encode()
        elif opcode == 2:
            # save log
            dbConnector.insert_log(msg, )
            response = "Success".encode()
            
        else: return "Failed!".encode()
        return response
    
    def handle(self):
        self.data = self.request.recv(16384)
        # print("new connection \n")
        if self.data.find(b';') != -1:
            ret = self.parse_message(self.data.split(b';')[0])
            self.request.sendall(ret)
            return
        self.request.sendall(self.data.upper())


if __name__ == "__main__":
    HOST, PORT = "0.0.0.0", 5001

    server = socketserver.TCPServer((HOST, PORT), MyTCPHandler)
    server.serve_forever()
    