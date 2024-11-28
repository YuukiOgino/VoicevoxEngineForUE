#!/bin/bash

if [ -z "$1" ]; then
    UEVERSION="5.2"
else
    UEVERSION="$1"
fi

CURRENT=$(cd $(dirname $0);pwd)
"/Users/Shared/Epic Games/UE_$UEVERSION/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh" -project="$CURRENT/VoicevoxEngineSample.uproject" -game
