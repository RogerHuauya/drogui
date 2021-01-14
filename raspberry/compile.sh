#!/bin/bash
var=$(git status)
set -- $var

for arg; do
  if [[ "${arg:0}" =~ ".." ]]; then
  continue;
  fi

  if [[ "${arg:0}" =~ ".h" ]];
  then 
    $(make cleanObjects)
  fi


  if [[ "${arg:0}" =~ ".cpp" ]];
  then 
    rm -f ${arg//.cpp/.o}
  fi

done

$(make)