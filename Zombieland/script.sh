#!/bin/bash
for file in tests/*
do
  if [ ${file: -3} == ".in" ]
  then
      echo ${file}
      echo -n "output programa: "
      cat $file | ./ejercicio
  else
      echo -n "output esperado: "
      cat $file
      echo ""
  fi
  done
