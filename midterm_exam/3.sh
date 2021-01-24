#!/bin/bash

SUM=0

for i in $* #명령행 인자를 하나씩 i에 넣으며 반복한다
do
 (( SUM+=i )) #SUM에 i를 더한 값을 다시 SUM에 저장한다
done

echo $SUM #SUM의 값을 출력한다
