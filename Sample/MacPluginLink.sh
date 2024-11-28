#!/bin/bash

CURRENT=$(cd $(dirname $0);pwd)
PLUGIN_DIR=$CURRENT/..
FULL_PATH=$(cd $PLUGIN_DIR && pwd)

ln -s $FULL_PATH/Plugins $CURRENT
