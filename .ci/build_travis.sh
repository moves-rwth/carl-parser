#!/usr/bin/env bash

pushd .

mkdir -p cache/

pushd cache/

if [ ! -d cache/carl ]; then
	git clone https://github.com/smtrat/carl.git
else
	pushd carl/
	git fetch
	git reset --hard origin/master
	popd
fi

mkdir -p carl/build
pushd carl/build/
cmake -D DEVELOPER=ON -D BUILD_ADDONS=OFF ../ || return 1
make lib_carl || return 1
popd

popd


mkdir -p build
cd build/

cmake ../ || return 1
make || return 1

./test/carl-parser-test || return 1

popd
