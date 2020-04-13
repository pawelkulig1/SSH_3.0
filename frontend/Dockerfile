from fedora

RUN dnf update -y && dnf install python3 mysql -y
RUN pip3 install flask mysql-connector-python flask-login flask-wtf

ENV FLASK_APP=main
ENV FLASK_ENV=development
WORKDIR /home

CMD ["flask", "run", "--host", "0.0.0.0"]
