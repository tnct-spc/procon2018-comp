TEMPLATE = app qt
CONFIG += console c++11
CONFIG -= app_bundle

QT += core gui widgets

SOURCES += \
        main.cpp \
    testagentparam.cpp \
    testevaluatespeed.cpp

HEADERS += \
    testagentparam.h \
    testevaluatespeed.h

LIBS += -L$$OUT_PWD/../Field/ -lField
INCLUDEPATH += $$PWD/../Field
DEPENDPATH += $$PWD/../Field

LIBS += -L$$OUT_PWD/../Visualizer/ -lVisualizer
INCLUDEPATH += $$PWD/../Visualizer
DEPENDPATH += $$PWD/../Visualizer

LIBS += -L$$OUT_PWD/../Solver/ -lSolver
INCLUDEPATH += $$PWD/../Solver
DEPENDPATH += $$PWD/../Solver
