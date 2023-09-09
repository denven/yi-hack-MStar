#!/bin/sh

if [ $# -ne 1 ]; then
    exit
fi

case $1 in
    ''|*[!0-9]*) exit;;
    *) continue;;
esac

if [ $1 -gt 99 ]; then
    exit
fi
if [ $1 -lt 1 ]; then
    exit
fi

MAX_AVI_NUMBER=10
USED_SPACE_LIMIT=$((100-$1))

cd /tmp/sd/record/timelapse
AVI_NUMBER=`ls -lt | grep .avi | awk 'END{print NR}'`
while [ "$AVI_NUMBER" -gt "$MAX_AVI_NUMBER" ]
do
    OLD_FILE=`ls -lt | grep .avi | awk 'END{print}' | awk '{print $9}'`
    if [ ! -z "$OLD_FILE" ]; then
        echo "Deleting file $OLD_FILE"
        rm -f $OLD_FILE
        AVI_NUMBER=`ls -lt | grep .avi | awk 'END{print NR}'`
    else
        break
    fi
done

cd /tmp/sd/record
USED_SPACE=`df -h /tmp/sd/ | grep mmc | awk '{print $5}' | tr -d '%'`

if [ -z "$USED_SPACE" ]; then
    exit
fi

while [ "$USED_SPACE" -gt "$USED_SPACE_LIMIT" ]
do
    OLD_DIR=`ls -lt | grep -v tmp | grep -v timelapse | tail -n1 | awk '{print $9}'`
    if [ ! -z "$OLD_DIR" ]; then
        echo "Deleting dir $OLD_DIR"
        rm -rf $OLD_DIR
    else
        break
    fi
    USED_SPACE=`df -h /tmp/sd/ | grep mmc | awk '{print $5}' | tr -d '%'`
done

echo "Done!"
