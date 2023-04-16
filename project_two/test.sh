#!/usr/bin/env bash

for i in {1..10}
do
    ../project_one/./ipkcpc -h 127.0.0.1 -p 2023 -m udp < tests/valid/"$i".in > tests/valid/"$i".out
    echo "test "$i" :"
    diff -s  tests/valid/"$i".out tests/valid/"$i".exp
done
