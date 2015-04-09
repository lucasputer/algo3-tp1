#!/bin/bash
for file in files/*
do
  if [ ${file: -3} == ".in" ]
  then
      cat $file | ./ej2-timer

  fi
  done
