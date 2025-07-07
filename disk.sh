#!/bin/bash

if [ ! -f "hd80M.img" ]; then
    yes | bximage -q -hd=80 -func=create -sectsize=512 -imgmode=flat build/hd80M.img
fi

fdisk ./build/hd80M.img <<EOF >/dev/null 2>&1
m
x
m
c
162
h
16
r
n
p
1
2048
33263
n
e
4
33264
163295
p
n
35312
51407
n
53456
76607
n
78656
91727
n
93776
121967
n
124016
163295
p
t
5
66
t
6
66
t
7
66
t
8
66
t
9
66
p
w
EOF

echo -e "\nPartition table created:"
fdisk -l build/hd80M.img
