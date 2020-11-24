#!/bin/bash

declare cleanCmake=1

if [ "$#" -eq 1 ] && [ "$1" == "1" ]; then
	cleanCmake=0
fi

if [ "$#" -eq 0 ]; then
	echo Do you want to clean CMake files ? "Y/N"

	read cleanYN
	
	if [ "$cleanYN" == "Y" ]; then
		cleanCmake=1
	else 
		cleanCmake=0
	fi		

fi

if [ "$cleanCmake" -eq "1" ]; then

	rm -Rf CMakeFiles

	rm cmake_install.cmake

	rm CMakeCache.txt

	rm Makefile

	cmake . 
fi

make
