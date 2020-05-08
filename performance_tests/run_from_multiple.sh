#!/bin/bash

for i in {1..25}
do
    out=$(docker run --rm --network ssh_30_internal -dit -v $(pwd)/ssh:/root/.ssh -v $(pwd):/script -v /home/pablo/Desktop/SSH_3.0/ssh_server/openssh-portable-server:/home --name ubuntu_$i ssh_server_development bash)
    #docker container exec -dit $out bash -c 'ls /home'
    docker container exec -dit $out bash -c '/script/perform_requests.sh 2&>1 > /script/out_'$out & 
done
wait

#docker container kill $(docker container ls | grep "ubuntu_" | cut -f1 -d " ")
