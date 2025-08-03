#!/bin/bash
count=10
i=1
should_make_dir=true

while $should_make_dir; do
  new_dir="arch""$i"
  mkdir $new_dir
  cd $new_dir
  echo "$i. I use Arch btw." > arch.txt
  cat arch.txt
  if (( i >= count )); then
    should_make_dir=false;
  fi
  ((i++))
done
