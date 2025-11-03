#!/bin/sh
cd "`dirname "$0"`"
AFS=./osx-x64/UnrealAndroidFileTool
ADB=
if [ "$ANDROID_HOME" != "" ]; then ADB=$ANDROID_HOME/platform-tools/adb; else ADB=/Users/chelae/Library/Android/sdk/platform-tools/adb; fi
DEVICE=
if [ "$1" != "" ]; then DEVICE="-s $1"; fi
echo
echo Uninstalling existing application. Failures here can almost always be ignored.
$ADB $DEVICE uninstall com.YourCompany.AlienDoc_
echo
echo Installing existing application. Failures here indicate a problem with the device \(connection or storage permissions\) and are fatal.
$ADB $DEVICE install AlienDoc-arm64.apk
if [ $? -eq 0 ]; then
	echo
	$ADB $DEVICE shell pm list packages com.YourCompany.AlienDoc_
	$ADB $DEVICE shell pm grant com.YourCompany.AlienDoc_ android.permission.FOREGROUND_SERVICE >/dev/null 2>&1
	$ADB $DEVICE shell pm grant com.YourCompany.AlienDoc_ android.permission.FOREGROUND_SERVICE_DATA_SYNC >/dev/null 2>&1
	$ADB $DEVICE shell pm grant com.YourCompany.AlienDoc_ android.permission.POST_NOTIFICATIONS >/dev/null 2>&1
	echo
	echo Removing old data. Failures here are usually fine - indicating the files were not on the device.
	$ADB $DEVICE shell 'rm -r $EXTERNAL_STORAGE/UnrealGame/AlienDoc_'
	$ADB $DEVICE shell 'rm -r $EXTERNAL_STORAGE/UnrealGame/UECommandLine.txt'
	$ADB $DEVICE shell 'rm -r $EXTERNAL_STORAGE/obb/com.YourCompany.AlienDoc_'
	$ADB $DEVICE shell 'rm -r $EXTERNAL_STORAGE/Android/obb/com.YourCompany.AlienDoc_'
	$ADB $DEVICE shell 'rm -r $EXTERNAL_STORAGE/Download/obb/com.YourCompany.AlienDoc_'
	echo
	echo Installing new data. Failures here indicate storage problems \(missing SD card or bad permissions\) and are fatal.
	STORAGE=$(echo "`$ADB $DEVICE shell 'echo $EXTERNAL_STORAGE'`" | cat -v | tr -d '^M')
	$AFS $DEVICE -p com.YourCompany.AlienDoc_ -k DF62B961E14A90D9F1B129B97F81D3F0 push main.1.com.YourCompany.AlienDoc_.obb '^mainobb'
	if [ $? -eq 0 ]; then




$AFS $DEVICE -p com.YourCompany.AlienDoc_ -k DF62B961E14A90D9F1B129B97F81D3F0 stop-all
	echo Grant READ_EXTERNAL_STORAGE and WRITE_EXTERNAL_STORAGE to the apk for reading OBB or game file in external storage.
	$ADB $DEVICE shell pm grant com.YourCompany.AlienDoc_ android.permission.READ_EXTERNAL_STORAGE >/dev/null 2>&1
	$ADB $DEVICE shell pm grant com.YourCompany.AlienDoc_ android.permission.WRITE_EXTERNAL_STORAGE >/dev/null 2>&1

		echo
		echo Installation successful
		exit 0
	fi
fi
echo
echo There was an error installing the game or the obb file. Look above for more info.
echo
echo Things to try:
echo 'Check that the device (and only the device) is listed with \"$ADB devices\" from a command prompt.'
echo Make sure all Developer options look normal on the device
echo Check that the device has an SD card.
exit 1
