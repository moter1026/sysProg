#!/bin/bash
mkdir A
cd A
touch 1.txt
ln 1.txt 2.txt
mkdir B
cd B
dd if=<(printf '\xFF\xFF\xFF\xFF') of=one.bin bs=1M count=10
ln -s one.bin 3.txt
mkdir C
cd C
mkdir D
cd D
head -c 500 /dev/urandom > rand.bin
cd ..
mkdir E
cd E
touch 4.txt
touch 5.txt
head -c 500 /dev/zero > zero.bin

