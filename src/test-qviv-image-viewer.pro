! include( common.pri ) {
    error( Couldn't find the common.pri file! )
}

HEADERS = QvivImageViewer.h MyApp.h
SOURCES = test-qviv-image-viewer.cc QvivImageViewer.cc 
CONFIG += qt debug 

