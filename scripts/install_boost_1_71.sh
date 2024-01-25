#!/usr/bin/env sh
set -e

BOOST_VER=1.71.0
BOOST_VERSION=1_71_0
DLDIR="$GITHUB_WORKSPACE/solidity/deps/downloads"
mkdir -p "$DLDIR"
(
    cd "$DLDIR"
	BOOST_TAR="boost_$BOOST_VERSION.tar.bz2"

	if [ ! -f "$SOFTWARE_TAR" ]; then
    	echo "Downloading $SOFTWARE_TAR..."
    	wget "https://boostorg.jfrog.io/artifactory/main/release/$BOOST_VER/source/$SOFTWARE_TAR"
	else
    	echo "$SOFTWARE_TAR already exists, skipping download."
	fi

	tar xvjf "$SOFTWARE_TAR"
    cd boost_$BOOST_VERSION
   	./bootstrap --prefix="$GITHUB_WORKSPACE/solidity/build/deps"
    ./b2
    ./b2 install
)
rm -rf "$DLDIR/boost_$BOOST_VERSION"
