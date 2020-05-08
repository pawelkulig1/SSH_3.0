#!/bin/bash

echo $EPOCHREALTIME
for i in {1..1000}; 
do 
    /home/ssh root@ssh_server ls; 
done
echo $EPOCHREALTIME
