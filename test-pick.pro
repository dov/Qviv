! include( common.pri ) {
    error( Couldn't find the common.pri file! )
}

HEADERS += TestPickQvivWidget.h
SOURCES += TestPickQvivWidget.cc test-pick.cc

CONFIG += qt debug 

