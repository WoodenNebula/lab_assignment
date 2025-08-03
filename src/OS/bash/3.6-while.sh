#!/bin/bash
echo "Printing 0-9"
i=0
while (( i < 10 )); do
  echo -n "$i "
  i=$(($i+1))
done
echo
