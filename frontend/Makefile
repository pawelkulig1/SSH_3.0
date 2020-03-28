run:
	docker run -v $$(pwd):/home -p5000:5000 --rm -it --network internal --ip 172.16.16.5 flask_frontend
build:
	docker build --tag flask_frontend .
	
