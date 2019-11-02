run:
	docker run --network internal --name ssh_server -dit -p 25:22 ssh_server_production

run_devel:
	docker run --rm --privileged --name ssh_server -it -v $$(pwd)/openssh-portable-server:/home -p 25:22 ssh_server_development

build:
	docker build --tag ssh_server_production -f Dockerfile_production .

build_devel:
	docker build --tag ssh_server_development -f Dockerfile_development .

prune:
	-docker container kill $$(docker container ls -q) 2> /dev/null
	-docker container rm $$(docker container ls -qa) 2> /dev/null


