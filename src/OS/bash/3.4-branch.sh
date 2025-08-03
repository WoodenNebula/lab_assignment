#!/bin/bash
read -p "Enter a number: " number

remainder=$(($number%2))

if [[ ($number == 0) ]]; then
  echo "The number is Zero."
elif [[ ($remainder == 0) ]]; then
  echo "The number is Even."
else
  echo "The number is Odd."
fi
