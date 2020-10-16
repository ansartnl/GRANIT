#!/bin/bash

CURRENT_DIR=$PWD

PROJECT_NAME=snmp

if [ -z "$1" ]
then
	DEST_DIR=$CURRENT_DIR/$PROJECT_NAME"_src"
else
	DEST_DIR=$1
fi

SCRIPT=$( readlink -f $0 )
PROJECT_DIR=$( dirname $SCRIPT )

echo Destination directory: $DEST_DIR

rm -rf $DEST_DIR

mkdir $DEST_DIR

# $PROJECT_NAME

mkdir $DEST_DIR/snmp

cp -f $PROJECT_DIR/snmp.pro $DEST_DIR/$PROJECT_NAME/
cp -f $PROJECT_DIR/release_notes $DEST_DIR/$PROJECT_NAME/
cp -f $PROJECT_DIR/version $DEST_DIR/$PROJECT_NAME/
cp -rf $PROJECT_DIR/install $DEST_DIR/$PROJECT_NAME/

# sed 's/svnversion %ROJECT_DIR%/echo '$( svnversion $PROJECT_DIR )'/g' $PROJECT_DIR/build.bat > $DEST_DIR/$PROJECT_NAME/build.bat
sed 's/$( svnversion )/'$( svnversion $PROJECT_DIR )'/g' $PROJECT_DIR/build.sh > $DEST_DIR/$PROJECT_NAME/build.sh
chmod +x $DEST_DIR/$PROJECT_NAME/build.sh

# netSnmpClient

mkdir $DEST_DIR/$PROJECT_NAME/netSnmpClient
mkdir $DEST_DIR/$PROJECT_NAME/netSnmpClient/lib

cp -f $PROJECT_DIR/netSnmpClient/netSnmpClient.pri $DEST_DIR/$PROJECT_NAME/netSnmpClient/
cp -f $PROJECT_DIR/netSnmpClient/netSnmpClient.pro $DEST_DIR/$PROJECT_NAME/netSnmpClient/
cp -rf $PROJECT_DIR/netSnmpClient/include $DEST_DIR/$PROJECT_NAME/netSnmpClient
cp -rf $PROJECT_DIR/netSnmpClient/src $DEST_DIR/$PROJECT_NAME/netSnmpClient

# netSnmpManager

mkdir $DEST_DIR/$PROJECT_NAME/netSnmpManager
mkdir $DEST_DIR/$PROJECT_NAME/netSnmpManager/lib

cp -f $PROJECT_DIR/netSnmpManager/netSnmpManager.pri $DEST_DIR/$PROJECT_NAME/netSnmpManager/
cp -f $PROJECT_DIR/netSnmpManager/netSnmpManager.pro $DEST_DIR/$PROJECT_NAME/netSnmpManager/
cp -rf $PROJECT_DIR/netSnmpManager/include $DEST_DIR/$PROJECT_NAME/netSnmpManager
cp -rf $PROJECT_DIR/netSnmpManager/src $DEST_DIR/$PROJECT_NAME/netSnmpManager

# netSnmpSubagent

mkdir $DEST_DIR/$PROJECT_NAME/netSnmpSubagent

cp -f $PROJECT_DIR/netSnmpSubagent/netSnmpSubagent.pro $DEST_DIR/$PROJECT_NAME/netSnmpSubagent/
cp -rf $PROJECT_DIR/netSnmpSubagent/etc $DEST_DIR/$PROJECT_NAME/netSnmpSubagent
cp -rf $PROJECT_DIR/netSnmpSubagent/share $DEST_DIR/$PROJECT_NAME/netSnmpSubagent
cp -rf $PROJECT_DIR/netSnmpSubagent/src $DEST_DIR/$PROJECT_NAME/netSnmpSubagent

# netSnmpTrapHandler

mkdir $DEST_DIR/$PROJECT_NAME/netSnmpTrapHandler

cp -f $PROJECT_DIR/netSnmpTrapHandler/netSnmpTrapHandler.pro $DEST_DIR/$PROJECT_NAME/netSnmpTrapHandler/
cp -rf $PROJECT_DIR/netSnmpTrapHandler/src $DEST_DIR/$PROJECT_NAME/netSnmpTrapHandler

# testClient

mkdir $DEST_DIR/$PROJECT_NAME/testClient

cp -f $PROJECT_DIR/testClient/testClient.pro $DEST_DIR/$PROJECT_NAME/testClient/
cp -rf $PROJECT_DIR/testClient/src $DEST_DIR/$PROJECT_NAME/testClient

# testManager

mkdir $DEST_DIR/$PROJECT_NAME/testManager

cp -f $PROJECT_DIR/testManager/testManager.pro $DEST_DIR/$PROJECT_NAME/testManager/
cp -rf $PROJECT_DIR/testManager/src $DEST_DIR/$PROJECT_NAME/testManager

# share

cp -rf $PROJECT_DIR/share $DEST_DIR/$PROJECT_NAME

# common

mkdir $DEST_DIR/common

COMMON_SUBPROJECTS=(
cmdline
enumstreaming
initmain
log
log4qt
nitaxmlsettings
qtservice
qtxmlsettings
scriptrunner
version
)

for SUBPROJECT in ${COMMON_SUBPROJECTS[*]}
do
    cp -rf $PROJECT_DIR/../common/$SUBPROJECT $DEST_DIR/common
done

# 3rdparty

mkdir $DEST_DIR/3rdparty

ZRDPARTY_SUBPROJECTS=(
qcorecmdlineargs
)

for SUBPROJECT in ${ZRDPARTY_SUBPROJECTS[*]}
do
    cp -rf $PROJECT_DIR/../3rdparty/$SUBPROJECT $DEST_DIR/3rdparty
done

# deleting .svn

find $DEST_DIR -name '.svn' -type d -exec rm -rf {} \; 1>/dev/null 2>&1
