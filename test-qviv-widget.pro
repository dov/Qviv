! include( common.pri ) {
    error( Couldn't find the common.pri file! )
}

HEADERS = QvivImageViewer.h QvivWidget.h QvivData.h
SOURCES = test-qviv-widget.cc QvivImageViewer.cc QvivWidget.cc QvivData.cc
CONFIG += qt debug 

