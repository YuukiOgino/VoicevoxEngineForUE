#!/bin/bash

UEVERSION="5.2"
CURRENT=$(cd $(dirname $0);pwd)
"/Users/Shared/Epic Games/UE_$UEVERSION/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh" -project=$CURRENT/VoicevoxEngineSample.uproject -game
