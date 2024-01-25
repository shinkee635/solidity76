#!/usr/bin/env sh
set -e

BOOST_VER=1.73.0
BOOST_VERSION=1_73_0
echo "LD_LIBRARY_PATH=$GITHUB_WORKSPACE/solidity/build/deps/lib" >> $GITHUB_ENV
echo "CPATH=$GITHUB_WORKSPACE/solidity/build/deps/include" >> $GITHUB_ENV
DLDIR="$GITHUB_WORKSPACE/solidity/deps/downloads"
mkdir -p "$DLDIR"
(
    cd "$DLDIR"
	BOOST_TAR="boost_$BOOST_VERSION.tar.bz2"

	if [ ! -f "$BOOST_TAR" ]; then
    	echo "Downloading $BOOST_TAR..."
    	wget "https://boostorg.jfrog.io/artifactory/main/release/$BOOST_VER/source/$BOOST_TAR"
	else
    	echo "$BOOST_TAR already exists, skipping download."
	fi

	tar xvjf "$BOOST_TAR"
    cd boost_$BOOST_VERSION
   	./bootstrap.sh --prefix="$GITHUB_WORKSPACE/solidity/build/deps"
    ./b2
    ./b2 install
)
rm -rf "$DLDIR/boost_$BOOST_VERSION"
