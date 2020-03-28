import mysql.connector
import time

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

    def get_keys(self, page=0):
        query = "SELECT id, host_name, pub_key, last_renewal, renewable_by, valid_through \
                FROM ssh_keys LIMIT {} OFFSET {};" \
                .format(20, (page) * 20)
        self.cursor.execute(query)
        data = []
        for data_tuple in self.cursor:
            data.append(list(data_tuple))

        return data

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

    def add_key(self, key):
        user = key.rsplit("= ")[1] #possible == also
        query = "INSERT INTO ssh_keys (host_name, pub_key, renewable_by, valid_through) VALUES('{}', '{}', FROM_UNIXTIME({}), FROM_UNIXTIME({}));" \
                .format(user, key, int(time.time()) + DBConnector.renewability_period, int(time.time()) + DBConnector.validity_period)
        return self.cursor.execute(query)