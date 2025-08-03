#!/bin/bash

# Check if ko and device exist

if lsmod | grep -q "^doggo_char"; then
    echo "Module exists, unloading it rn"
    sudo rmmod -f doggo_char
fi    

if [[ -c /dev/doggo_char ]]; then
    echo "Device node exists, deleting it rn"
    sudo rm /dev/doggo_char
fi

cd ../build
insmod doggo_char.ko
sudo mknod /dev/doggo_char c 240 0
sudo chmod 666 /dev/doggo_char

echo "Hello doggo" > /dev/doggo_char
result=$(cat /dev/doggo_char)

if [[ "$result" == "Hello doggo" ]];then
    echo "DOGGO IS PLEASED, YOU PASS"
else
    echo "DOGGO IS UNHAPPY, YOU FAIL"
fi
