FROM ubuntu
RUN apt-get update -y
RUN apt-get install python3 python3-pip libmysqlclient-dev -y
WORKDIR /home
COPY requirements.txt requirements.txt
RUN pip3 install -r requirements.txt

CMD ["python3", "main.py"]
