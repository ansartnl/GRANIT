#!/bin/bash

CURRENT_DIR=$PWD

PROJECT_NAME=snmp

if [ -z "$1" ]
then
    RELEASE_DIR=./NetSnmpProject
else
    RELEASE_DIR=$1
fi

SCRIPT=$( readlink -f $0 )
PROJECT_DIR=$( dirname $SCRIPT )

BUILD_DIR=$PROJECT_DIR/build
VERSION_PATH=$PROJECT_DIR/../common/version

MAJOR_VERSION=0
MINOR_VERSION=0
REVISION_VERSION=$( svnversion )

ALL_VERSION=`cat $PROJECT_DIR/version `
MAJOR_VERSION=`echo $ALL_VERSION | sed 's/\s[0-9]//g'`
MINOR_VERSION=`echo $ALL_VERSION | sed 's/[0-9]\s//g'`
echo Version: $MAJOR_VERSION.$MINOR_VERSION.$REVISION_VERSION

# version.h

rm -f "$VERSION_PATH/version.tmp"
mv -f "$VERSION_PATH/version.h" "$VERSION_PATH/version.tmp"

sed 's/__VERSION_MAJOR__/'$MAJOR_VERSION'/g' "$VERSION_PATH/version.macros" > "$VERSION_PATH/version.m1"
sed 's/__VERSION_MINOR__/'$MINOR_VERSION'/g' "$VERSION_PATH/version.m1" > "$VERSION_PATH/version.m2"
sed 's/__VERSION_REVISION__/'$REVISION_VERSION'/g' "$VERSION_PATH/version.m2" > "$VERSION_PATH/version.h"

rm -f "$VERSION_PATH/version.m1"
rm -f "$VERSION_PATH/version.m2"

echo Version file changed

# building

echo Building...

rm -rf "$BUILD_DIR"
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

qmake "$PROJECT_DIR/$PROJECT_NAME.pro" -recursive CONFIG+=release CONFIG+=deploy
if [ $? -ne 0 ] 
then
    exit 1
fi
make --quiet
if [ $? -ne 0 ] 
then
    exit 1
fi

cd "$PROJECT_DIR"

echo Build finished

# cleaning

rm -f "$VERSION_PATH/version.h"
mv -f "$VERSION_PATH/version.tmp" "$VERSION_PATH/version.h"

rm -rf "$BUILD_DIR"

echo Cleaned

# copying

#echo Starting update...

#./update.sh "$RELEASE_DIR"

#echo Update finished

cd "$CURRENT_DIR"

