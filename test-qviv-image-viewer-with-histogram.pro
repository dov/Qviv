! include( common.pri ) {
    error( Couldn't find the common.pri file! )
}

HEADERS += MyHistApp.h
SOURCES += test-qviv-image-viewer-with-histogram.cc
CONFIG += qt debug 

