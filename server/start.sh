#!/bin/bash

make

port="10000"
time="5"
threads="6"

./startup.o $port $time $threads