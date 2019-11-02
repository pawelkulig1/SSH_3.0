sleep 60 &&
mysql -h 172.16.16.3 -uroot -ppass -e "create database ssh" &&
mysql -h 172.16.16.3 -uroot -ppass ssh < create_table.sql &

