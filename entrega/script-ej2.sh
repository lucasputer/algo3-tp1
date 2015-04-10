#!/bin/bash
for file in tests2/*
do
  if [ ${file: -3} == ".in" ]
  then
      echo ${file}
      echo -n "output programa: "
      cat $file | ./ejercicio2
  else
      echo -n "output esperado: "
      cat $file
      echo ""
  fi
  done
