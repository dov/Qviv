! include( common.pri ) {
    error( "Couldn't find the common.pri file!" )
}
CONFIG += qt debug 
QT += widgets

SOURCES += qviv.cc 

