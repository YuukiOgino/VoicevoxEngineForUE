#!/bin/bash

CURRENT=$(cd $(dirname $0);pwd)
echo $CURRENT
PLUGIN_DIR=$CURRENT/..
FULL_PATH=$(cd $PLUGIN_DIR && pwd)
echo $FULL_PATH

ln -s $FULL_PATH/Plugins $CURRENT
