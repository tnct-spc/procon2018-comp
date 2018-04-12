#-------------------------------------------------
#
# Project created by QtCreator 2018-04-10T08:46:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mejirodai
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mejirodai.cpp

HEADERS += \
        mejirodai.h

FORMS += \
        mejirodai.ui

LIBS += -L$$OUT_PWD/../Field/ -lField
INCLUDEPATH += $$PWD/../Field
DEPENDPATH += $$PWD/../Field

LIBS += -L$$OUT_PWD/../Solver/ -lSolver
INCLUDEPATH += $$PWD/../Solver
DEPENDPATH += $$PWD/../Solver


LIBS += -L$$OUT_PWD/../Visualizer/ -lVisualizer
INCLUDEPATH += $$PWD/../Visualizer
DEPENDPATH += $$PWD/../Visualizer
