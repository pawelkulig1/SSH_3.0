#!/bin/bash

for i in {1..100}
do
    docker run --rm -dit --name ubuntu_$i fedora bash
done