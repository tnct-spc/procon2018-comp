#-------------------------------------------------
#
# Project created by QtCreator 2018-04-04T00:14:03
#
#-------------------------------------------------

QT       -= gui

TARGET = Field
TEMPLATE = lib

DEFINES += FIELD_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += precompile_header
PRECOMPILED_HEADER = $$PWD/../precompile/precompile.h
precompile_header:!isEmpty(PRECOMPILED_HEADER) {
    DEFINES += USING_PCH
}
INCLUDEPATH += $$PWD/../precompile
DEPENDPATH += $$PWD/../precompile


SOURCES += \
        field.cpp \
        csvio.cpp \
        binaryio.cpp

HEADERS += \
        field.h \
        field_global.h \
        csvio.h \
        binaryio.h

