#sleep 60 &&
mysql -h 0.0.0.0 -uroot -ppass -e "drop database IF EXISTS ssh" &&
mysql -h 0.0.0.0 -uroot -ppass -e "create database ssh" &&
mysql -h 0.0.0.0 -uroot -ppass ssh < /home/create_table.sql

