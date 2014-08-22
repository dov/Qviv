! include( common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

SOURCES += test-qviv-widget.cc 

CONFIG += qt debug 
QT += widgets

