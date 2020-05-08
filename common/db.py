import mysql.connector
import time
from common.convert_key import convert_key_to_hex
from enum import Enum
import sys

class DBConnector:
    validity_period = None
    renewability_period = None

    def __init__(self):
        self.cnx = None
        self.cursor = None

        self._connect_db()
        self._get_config()

    def _get_config(self):
        query = "SELECT validity_period, renewability_period FROM ssh_config;"
        self.cursor.execute(query)
        data = self.cursor.fetchall()
        if len(data) == 0:
            DBConnector.validity_period, DBConnector.renewability_period = 0, 0
        else:
            DBConnector.validity_period, DBConnector.renewability_period = data[0][0], data[0][1] #*self.cursor[0]

    def _connect_db(self):
        self.cnx = mysql.connector.connect(user='root', password='pass', host="db", database="ssh")
        self.cursor = self.cnx.cursor()
    
    def __del__(self):
        self.cursor.close()
        self.cnx.close()

    def search_keys(self, search_q, page=0):
        query = "SELECT id, host_name, pub_key, last_renewal, renewable_by, valid_through \
                FROM ssh_keys WHERE (id RLIKE '{}' or host_name RLIKE '{}' or pub_key RLIKE '{}' or last_renewal RLIKE '{}' or renewable_by RLIKE '{}' or valid_through RLIKE '{}') \
                LIMIT {} OFFSET {};" \
                .format(search_q, search_q, search_q, search_q, search_q, search_q, 20, (page) * 20)
        print(search_q, query, file=sys.stderr)
        self.cursor.execute(query)
        data = []
        for data_tuple in self.cursor:
            data.append(list(data_tuple))

        return data

    def search_logs(self, search_q, page=0):
        query = "SELECT id, log, source, occured \
                FROM ssh_log WHERE (id RLIKE '{}' or log RLIKE '{}' or source RLIKE '{}' or occured RLIKE '{}') \
                ORDER BY occured ASC LIMIT {} OFFSET {};" \
                .format(search_q, search_q, search_q, search_q, 20, (page) * 20)
        try:
            self.cursor.execute(query)
            data = []
            for data_tuple in self.cursor:
                data.append(list(data_tuple))
            return data
        except Exception as e:
            return str(e)

    def delete_key(self, id):
        print("deleting key!", id)
        query = "DELETE from ssh_keys WHERE id={};".format(id)
        self.cursor.execute(query)
        return True

    def prolong(self, id):
        query = "UPDATE ssh_keys SET valid_through=FROM_UNIXTIME({}), renewable_by=FROM_UNIXTIME({}) WHERE id={}" \
            .format(int(time.time()) + DBConnector.validity_period, 
                    int(time.time()) + DBConnector.renewability_period, 
                    id)
        return self.cursor.execute(query)

    def force_renewal(self, id):
        query = "UPDATE ssh_keys SET renewable_by=FROM_UNIXTIME({}) WHERE id={}".format(int(time.time()), id)
        return self.cursor.execute(query)

    def disable_renewal(self, id):
        query = "UPDATE ssh_keys SET valid_through=0, renewable_by=0 WHERE id={}".format(id)
        return self.cursor.execute(query)

    def parse_public_key(self, key):
        padding_size = 0
        if key.find("==") != -1:
            padding_size = 2
        elif key.find("=") != -1:
            padding_size = 1
        else:
            padding_size = 0

        user = "user"
        beg = key.find(" ") + 1
        end = 0
        if padding_size == 1:
            end = key.rfind("=") + 1
        elif padding_size == 2:
            end = key.rfind("==") + 2
        else:
            return (None, None)
        
        ssh_key = key[beg:end]
        try:
            if key[end+1] == "=":
                user = key[end+2:]
            else:
                user = key[end+1:]
        except:
            user="user"

        ssh_key = ssh_key.replace(" ", "+")
        return ssh_key, user

    def add_key(self, key):
        print("add_key: ", key)
        ssh_key, user = self.parse_public_key(key)
        if ssh_key is None or user is None:
            return False

        key_converted = convert_key_to_hex(ssh_key)[:-6]

        query = "INSERT INTO ssh_keys (host_name, pub_key, pub_key_converted, \
                renewable_by, valid_through) VALUES('{}', '{}', '{}', \
                FROM_UNIXTIME({}), FROM_UNIXTIME({}));" \
                .format(user, ssh_key, key_converted.decode(), \
                        int(time.time()) + DBConnector.renewability_period, \
                        int(time.time()) + DBConnector.validity_period)

        return self.cursor.execute(query)

    def rotate_public_key(self, old_public, new_public):
        ssh_key, user = self.parse_public_key(new_public)
        key_converted = convert_key_to_hex(ssh_key)[:-6]
        old_public = old_public.split(" ")[1]
        new_public = new_public.split(" ")[1]
        
        query = "UPDATE ssh_keys SET  \
                    host_name='{}', \
                    pub_key='{}', \
                    pub_key_converted='{}', \
                    last_renewal=FROM_UNIXTIME({}), \
                    renewable_by=FROM_UNIXTIME({}), \
                    valid_through=FROM_UNIXTIME({}) \
                    WHERE pub_key='{}';" \
                    .format( \
                        user, \
                        new_public, \
                        key_converted.decode(), \
                        int(time.time()), \
                        int(time.time()) + DBConnector.renewability_period, \
                        int(time.time()) + DBConnector.validity_period, \
                        old_public)

        # print(query)
        return self.cursor.execute(query)

    def is_key_in_base(self, key_converted):
        NO_KEY_OR_INVALID = 0
        VALID             = 1
        NEEDS_RENEWAL     = 2

        query = "SELECT pub_key_converted, UNIX_TIMESTAMP(renewable_by), UNIX_TIMESTAMP(valid_through) FROM ssh_keys WHERE pub_key_converted='{}';".format(key_converted)
        self.cursor.execute(query)
        ret = self.cursor.fetchone()
        
        row_count = self.cursor.rowcount
        if row_count == 0 or ret is None:
            return 0

        pub_key_converted, renewable_by, valid_through = ret

        now = time.time()
        valid = 0
        renew = 0
        if (valid_through > now or valid_through == 0): 
            valid = 1
		
        if (renewable_by > now or renewable_by == 0): 
            renew = 1

        status = 0
        if (valid and renew):
            status = VALID
        elif not valid: 
            status = NO_KEY_OR_INVALID
        elif (valid and not renew): 
            status = NEEDS_RENEWAL
        
        return status
        
    def insert_log(self, source, log_msg):
        query = "INSERT INTO ssh_log (log, source, occured) VALUES ('{}', '{}', FROM_UNIXTIME({}));".format(log_msg, source, time.time())
        self.cursor.execute(query)
        self.cnx.commit()


