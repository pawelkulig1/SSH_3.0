run:
	make -C ../ create_network
	docker run --rm --network internal --ip 172.16.16.3 --name some-mariadb -v $$(pwd):/home -e MYSQL_ROOT_PASSWORD=pass -p 3306:3306 -d mariadb
#	sleep 60
#	docker exec some-mariadb /home/prepare_db.sh
#	./prepare_db.sh


prune:
	make -C ../ prune
