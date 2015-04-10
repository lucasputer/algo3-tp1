#!/bin/bash
for file in tests3/*
do
  if [ ${file: -3} == ".in" ]
  then
      echo ${file}
      echo -n "output programa: "
      cat $file | ./ejercicio3
  else
      echo -n "output esperado: "
      cat $file
      echo ""
  fi
  done
