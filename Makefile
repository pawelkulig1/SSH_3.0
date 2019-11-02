prepare_devel_env:
	create_network
	make run_mariadb_devel
	make run_ssh_server_devel

create_network:
	-docker network create --subnet 172.16.16.0/24 internal


prepare_env:
	-docker network create --subnet 172.16.16.0/24 internal
	make run_mariadb
	make run_ssh_server

build_ssh_server:
	make -C ssh_server/ build

run_ssh_server_devel:
	make -C ssh_server/ run_devel

run_mariadb:
	make -C database/

run_mariadb_devel:
	make -C database/

prune:
	-docker container kill $$(docker container ls -q) 2> /dev/null
	-docker container rm $$(docker container ls -qa) 2> /dev/null
	-docker network rm internal 2> /dev/null
