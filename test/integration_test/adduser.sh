#!/usr/bin/env bash
# run as root

if [[ $1 != "" && $2 != "" ]]; then
    sudo adduser $1 --gecos "First Last,RoomNumber,WorkPhone,HomePhone" --disabled-password 
    echo -e "$2\n$2\n" | sudo passwd $USER_TEST
else
    echo "Usage: adduser.sh USERNAME PASSWORD"
fi
