#! /bin/bash

if [ "$1" == "run" ]; then
	pushd bin
	./MyApplication
	popd
	exit
fi

mkdir -p build
pushd build &&\
cmake .. &&\
cmake --build .

