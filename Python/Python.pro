QT       += core gui widgets

TARGET = Python
TEMPLATE = lib

DEFINES += SOLVER_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
    communication.cpp

HEADERS += \
    communication.h

LIBS += -L$$OUT_PWD/../Field/ -lField
INCLUDEPATH += $$PWD/../Field
DEPENDPATH += $$PWD/../Field






LIBS += -lboost_python3
LIBS += -lpython3.6m

INCLUDEPATH += /usr/include/python3.6m
