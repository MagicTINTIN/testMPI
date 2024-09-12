#!/usr/bin/bash
echo -ne "$(tput setaf 9)"
if [[ $1 == "-quickprog" ]]
then
    slowcompile=" -O3 -flto -funroll-loops -mtune=native -march=native "
else
    slowcompile=""
fi
if [[ $1 == "-qcomp4" ]]
then
    slowcompile=" -O0 "
else
    slowcompile=""
fi

g++ ./jsonGenerator.cpp -o jsonGenerator.o $slowcompile
if [[ $? == 0 ]]; then
    echo "$(tput setaf 2)$(tput bold)Compilation of jsonGenerator finished.$(tput sgr0)"
fi

echo -ne "$(tput sgr0)"
