#!/bin/bash

echo "xl_mp43651"$3$4 >xl.txt

key=`md5sum xl.txt | awk '{print $1}'`

echo $key

echo "http://"$1$2"?key="$key"&key1="$4""

curl -r "0-10240" "http://"$1$2"?key="$key"&key1="$4""

#"Range: bytes=%-%\r\n"

