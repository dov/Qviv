! include( common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

SOURCES += test-svg.cc 

CONFIG += qt debug 
QT += widgets

