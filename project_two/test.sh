#!/usr/bin/env bash

for i in {1..10}
do
    ../project_one/./ipkcpc -h 127.0.0.1 -p 2023 -m udp < tests/valid/"$i".in > tests/valid/"$i".out
    echo "VALID test "$i" :"
    diff -s  tests/valid/"$i".out tests/valid/"$i".exp
done
for i in {1..10}
do
    ../project_one/./ipkcpc -h 127.0.0.1 -p 2023 -m udp < tests/not-valid/"$i".in > tests/not-valid/"$i".out
    echo "NOT-VALID test "$i" :"
    cut -c 1-4 tests/not-valid/"$i".out > tests/not-valid/"$i".part
    diff -s tests/not-valid/"$i".part tests/not-valid/0.exp
done

rm -rf *.part
