#!/bin/sh
if [ $? -ne 0]; then
echo "Required argument missing, pass a dump of adb crash log."
exit 1
fi
cd "`dirname "$0"`"
NDKSTACK=
if [ "$ANDROID_NDK_ROOT" != "" ]; then NDKSTACK=$%ANDROID_NDK_ROOT/ndk-stack; else ADB=/ndk-stack; fi
$NDKSTACK -sym AlienDoc__Symbols_v1/AlienDoc_arm64 -dump "%1" > AlienDoc__SymbolizedCallStackOutput.txt
exit 0
