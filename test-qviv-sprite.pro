! include( common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

SOURCES += test-qviv-sprite.cc 

CONFIG += qt debug 
QT += widgets

