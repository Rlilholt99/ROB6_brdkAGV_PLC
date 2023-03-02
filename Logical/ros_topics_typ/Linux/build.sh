#!/bin/sh

# Get the installed version of exos-data-eth
EXOS_DATA_PKG_NAME="exos-data-eth"
EXOS_DATA_VERSION_INSTALLED=$(dpkg -s $EXOS_DATA_PKG_NAME 2>/dev/null | grep -i version | cut -d" " -f2)
if [ -z $EXOS_DATA_VERSION_INSTALLED ] ; then
    # Fall-back to check the installed version of exos-data
    EXOS_DATA_PKG_NAME="exos-data"
    EXOS_DATA_VERSION_INSTALLED=$(dpkg -s $EXOS_DATA_PKG_NAME 2>/dev/null | grep -i version | cut -d" " -f2)
fi

# If there is nothing installed at all
if [ -z $EXOS_DATA_VERSION_INSTALLED ] ; then
    echo "ERROR: Did not find any version of $EXOS_DATA_PKG_NAME"
    echo "Please install exos-data-eth or exos-data in your build system:"
    echo "sudo ./setup_build_environment.sh"
    sleep 5
    exit 1
fi

## Check if no version is given as parameter to the script
if [ -z $1 ] ; then
    echo "WARNING: Version of $EXOS_DATA_PKG_NAME is $EXOS_DATA_VERSION_INSTALLED but required version is unknown"
    echo "Please use \$(EXOS_VERSION) in .exospkg BuildCommand Arguments when calling $0"

# Check compatibility of exos-data/exos-data-eth and exos version from technology package
elif [ "$1" != $EXOS_DATA_VERSION_INSTALLED ] ; then
    echo "ERROR: Version of $EXOS_DATA_PKG_NAME is $EXOS_DATA_VERSION_INSTALLED instead of required $1"
    echo "Please install the version $1 in your build system:"
    echo "sudo ./setup_build_environment.sh"
    sleep 5
    exit 1
fi

# Checks done, continue with the build

finalize() {
    cd ..
    rm -rf build/*
    rm -r build
    sync
    exit $1
}

mkdir build > /dev/null 2>&1
rm -rf build/*
gnome-terminal
cd build

cmake -Wno-dev ..
if [ "$?" -ne 0 ] ; then
    finalize 2
fi

make
if [ "$?" -ne 0 ] ; then
    finalize 3
fi

cpack
if [ "$?" -ne 0 ] ; then
    finalize 4
fi

cp -f exos-comp-ros_topics_typ_1.0.0_amd64.deb ..

cp -f _libros_topics_typ.so ..

cp -f libros_topics_typ.py ..

finalize 0
