! include( common.pri ) {
    error( Couldn't find the common.pri file! )
}
CONFIG += qt debug 

TEMPLATE = subdirs
SUBDIRS = qjson 

SOURCES += qviv.cc QvivWidget.cc 

