run:
	make -C ../ create_network
	docker run --rm --network internal --ip 172.16.16.3  --name some-mariadb -e MYSQL_ROOT_PASSWORD=pass -p 3306:3306 -d mariadb
	./prepare_db.sh

prune:
	make -C ../ prune
