TEMPLATE = app qt
CONFIG += console c++11
CONFIG -= app_bundle

QT += core gui widgets

SOURCES += \
        main.cpp \
    testagentparam.cpp \
    testevaluatespeed.cpp \
    testgetfielddata.cpp

HEADERS += \
    testagentparam.h \
    testevaluatespeed.h \
    testgetfielddata.h

LIBS += -L$$OUT_PWD/../Field/ -lField
INCLUDEPATH += $$PWD/../Field
DEPENDPATH += $$PWD/../Field

LIBS += -L$$OUT_PWD/../Visualizer/ -lVisualizer
INCLUDEPATH += $$PWD/../Visualizer
DEPENDPATH += $$PWD/../Visualizer

LIBS += -L$$OUT_PWD/../Solver/ -lSolver
INCLUDEPATH += $$PWD/../Solver
DEPENDPATH += $$PWD/../Solver

LIBS += -lboost_python3
LIBS += -lpython3.6m

INCLUDEPATH += /usr/include/python3.6m
