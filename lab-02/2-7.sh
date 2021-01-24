#!/bin/bash

cnt=2

while ((cnt <= 9))
do
    i=1
    while ((i <= 9))
    do
        if ((i!=9))
        then
            printf "%2d " $((i*cnt))
        else
            printf "%2d\n" $((i*cnt))
        fi
        ((i=i+1))
    done
    ((cnt=cnt+1))
done
