#!/bin/bash

HOUR=`date +%H` #현재 시간 저장

if (($HOUR < 12)) #12시 이전일 경우
then
    printf "G M.\n"
elif (($HOUR < 18)) #18시 이전일 경우
then
    printf "G A.\n"
elif (($HOUR < 22))#22시 이전일 경우
then
    printf "G E.\n"
else
    printf "G N.\n"
fi

exit 0
