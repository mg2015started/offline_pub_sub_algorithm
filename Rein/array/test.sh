#!/bin/bash
ulimit -s unlimited
cd Rein
g++ -std=c++11 chrono_time.h data_structure.h generator.h generator.cpp oldrein.h oldrein.cpp rein.h rein.cpp printer.h printer.cpp util.h util.cpp main.cpp -o Rein
echo "finish compile"
for ((i=1; i<=24; i++))
do 
	./Rein $i
	echo "finish test $i"
done
