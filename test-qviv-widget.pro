! include( common.pri ) {
    error( Couldn't find the common.pri file! )
}

HEADERS = QvivImageViewer.h QvivWidget.h
SOURCES = test-qviv-widget.cc QvivImageViewer.cc QvivWidget.cc
CONFIG += qt debug 

