import mysql.connector as mariadb

connection = mariadb.connect(user='root', password='pass', database='ssh', host="172.16.16.3")
cursor = connection.cursor()

def alter_key(key):
    i=0
    while key[i] == 'f':
        i+=1
    key[i] = str(hex(int(key[i], 16) + 1))[2]
    return key #"".join(key)

key = ['0']*768
#print(alter_key(default_key))


for i in range(1000):
    key = alter_key(key)
    #print("".join(key))
    cursor.execute("INSERT INTO ssh_keys (host_name, pub_key, pub_key_converted) values ('root', 'ssh-rsa aaaab3nzac1yc2eaaaadaqabaaabgqdzyfskc4udbaimf30manv6jc8k/5zpqarliidlzgtfqmkwyyv4hjuljwrnyl3cik+jbpwgunrj1tp6fdo6gjnmmgd3+ck9ixrxxy55qkxom1r0frbqrlzc6xcqghzhlacmxcnopy3+wr2iem6w0z2zdnjrexo1is8rhkxes08g43ke9xnc9zvdmtzvdfwn64qw30okxs5vdyv65p16db+6n2v1c30lw3hzapstajusjbmfbh54mk1ar6ylzl/mh0lwytrlxbo43ystpn6vsfbryqu+236ysj2s57+6eeo8ilffzprf+1m3rfljoc7pglil7xyezoijooqlxt1ep2vehuxwbqusimhqbbeuf38xhf9knd7pk/sizw65gp6aienl4rl1jqqro0++quatcloshida9bzfktpioov84w60wxh4nfwa70houcxhapldixbhpmsapsfka22msds3c7fkjutfdnh4vpvyw9zg+7qkssrejrm7+w40i3fvtktk1vs= pablo2@localhost.localdomain', '" + "".join(key) + "')")

connection.commit()
connection.close()
