! include( common.pri ) {
    error( "Couldn't find the common.pri file!" )
}
CONFIG += qt debug
QT += widgets
QMAKE_CXXFLAGS += -std=c++11

SOURCES += qviv.cc QvivParser.cc
LIBS += -lexpat
