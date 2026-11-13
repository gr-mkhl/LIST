#!/bin/bash

cd "$(dirname "$0")/.." || exit

mkdir -p build

g++ -D _DEBUG -DDEBUGGING -DLOGGING -I include src/list.c src/main.c src/log.c src/list_dump.c src/tests.c -o build/list.out

./build/list.out
