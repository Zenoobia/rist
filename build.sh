#! /bin/bash

pushd () {
	command pushd "$@" > /dev/null 
}

popd () { 
	command popd "$@" > /dev/null 
}

if [ "$1" == "run" ]; then
	pushd bin
	./MyApplication
	popd
	exit
fi

if [ "$1" == "test" ]; then
	pushd bin
	./MyTest
	popd
	exit
fi



mkdir -p build
pushd build &&\
cmake .. &&\
make
#cmake --build .

