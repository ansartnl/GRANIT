#!/bin/bash

SCRIPT_PATH=`dirname $0`
SCRIPT_PATH=`dirname $SCRIPT_PATH`
OUTPUT_PATH=$1
BUILDDIR=release

cd $SCRIPT_PATH

# Building 'supervisor' solution package

if [ -d $BUILDDIR ];
then
    rm -rf $BUILDDIR
fi

mkdir $BUILDDIR

echo "Start building ..."

cd $BUILDDIR
qmake $SCRIPT_PATH/supervisor.pro CONFIG+=release
make
cd ..

echo "Build finished ..."

# Copy files for distribution ...

echo "Copy supervisor binaries"
cp -P $SCRIPT_PATH/gui/bin/SupervisorMonitor $OUTPUT_PATH/bin/
cp -P $SCRIPT_PATH/gui/share/translations/*.qm $OUTPUT_PATH/share/translations/
cp -P $SCRIPT_PATH/gui/etc/config.xml $OUTPUT_PATH/etc/SupervisorMonitor.xml
cp -P $SCRIPT_PATH/server/bin/SupervisorServer $OUTPUT_PATH/bin/
cp -P $SCRIPT_PATH/server/etc/config.xml $OUTPUT_PATH/etc/SupervisorServer.xml

cd ..	# cd $SCRIPT_PATH

