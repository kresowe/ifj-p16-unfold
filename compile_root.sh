#!/bin/bash
source ~/root_build/bin/thisroot.sh
if [ $# != 1 ]; then
	echo "Potrzeba 1 argumentu wejsciowego:"
	echo "nazwa pliku out"
else 
	echo g++ -Wall -std=c++11 -I `root-config --incdir` *.cpp -o $1 `root-config --libs`
	g++ -Wall -std=c++11 -I `root-config --incdir` *.cpp -o $1 `root-config --libs`
fi
