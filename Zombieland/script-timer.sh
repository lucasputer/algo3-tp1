#!/bin/bash
for file in files/*
do
  if [ ${file: -3} == ".txt" ]
  then
      echo ${file}
      echo -n "tamanio - tiempo: "
      cat $file | ./ejercicio-timer

  fi
  done
