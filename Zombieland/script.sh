#!/bin/bash
for file in files/*
do
  if [ ${file: -3} == ".in" ]
  then
      cat $file | ./ejercicio1-timer

  fi
  done
