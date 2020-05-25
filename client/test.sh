#!/bin/bash

make

address="127.0.0.1"
port="10000"
test="1000"

./startup.o $address $port $test
