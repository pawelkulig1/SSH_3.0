#!/bin/bash

for i in {1..100}
do
	ssh root@172.17.0.2 -p 22 ls > output.txt
done
