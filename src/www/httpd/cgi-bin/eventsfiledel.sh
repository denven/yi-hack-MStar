#!/bin/sh

validateRecFile()
{
    if [ "${#1}" != "27" ]; then
        FILE="none"
    fi

    if [ "Y${1:4:1}" != "YY" ] ; then
        FILE="none"
    fi
    if [ "M${1:7:1}" != "MM" ] ; then
        FILE="none"
    fi
    if [ "D${1:10:1}" != "DD" ] ; then
        FILE="none"
    fi
    if [ "H${1:13:1}" != "HH" ] ; then
        FILE="none"
    fi

    if [ "M${1:17:1}" != "MM" ] ; then
        FILE="none"
    fi
    if [ "S${1:20:1}" != "SS" ] ; then
        FILE="none"
    fi
}

fbasename()
{
    echo ${1:0:$((${#1} - 4))}
}

YI_HACK_PREFIX="/home/yi-hack"

. $YI_HACK_PREFIX/www/cgi-bin/validate.sh

if ! $(validateQueryString $QUERY_STRING); then
    printf "Content-type: application/json\r\n\r\n"
    printf "{\n"
    printf "\"%s\":\"%s\"\\n" "error" "true"
    printf "}\n"
    exit
fi

FILE="none"

CONF="$(echo $QUERY_STRING | cut -d'=' -f1)"
VAL="$(echo $QUERY_STRING | cut -d'=' -f2)"

if [ "$CONF" == "file" ] ; then
    FILE="$VAL"
fi

validateRecFile $FILE

if [ "$FILE" == "none" ] ; then
    printf "Content-type: application/json\r\n\r\n"
    printf "{\n"
    printf "\"%s\":\"%s\"\\n" "error" "true"
    printf "}\n"
    exit
fi

BASE_NAME=$(fbasename "$FILE")
rm -f /tmp/sd/record/$BASE_NAME.mp4
rm -f /tmp/sd/record/$BASE_NAME.jpg

printf "Content-type: application/json\r\n\r\n"

printf "{\n"
printf "\"%s\":\"%s\"\\n" "error" "false"
printf "}\n"
