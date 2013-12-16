#!/bin/sh

echo "$1"
if [ "$1" = "project" ] ; then
	qmake-qt4 -project "DEFINES += DW_RAYTRACER_GUI_ENABLED"
fi
qmake-qt4
make
