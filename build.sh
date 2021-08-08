#!/bin/bash
################################################################################
#
#	build.sh
#
#	Build script for hoxeysky.  This script will compile and upload hoxeysky
#	to an Arduino Uno.  This is perfectly doable from within the Arduino IDE,
#	but honestly it kind of sucks.
#
#	Copyright 2021 Brian Murphy
#	https://github.com/gurudvlp
#
################################################################################

arduino-cli compile -b arduino:avr:uno .
arduino-cli upload -b arduino:avr:uno -p /dev/ttyACM0 .
