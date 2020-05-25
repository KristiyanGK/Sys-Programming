#!/bin/bash

make

address="127.0.0.1"
port="10000"
test="150"

counter=0
while [ $counter -le 3 ]
do
    echo start $counter \n
    ./startup.o $address $port $test
    echo end $counter \n
    ((counter++))
done
