TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    communication.cpp

HEADERS += \
    communication.h
LIBS += -L$$OUT_PWD/../Field/ -lField
INCLUDEPATH += $$PWD/../Field
DEPENDPATH += $$PWD/../Field

LIBS += -L$$OUT_PWD/../Solver/ -lSolver
INCLUDEPATH += $$PWD/../Solver
DEPENDPATH += $$PWD/../Solver


LIBS += -L$$OUT_PWD/../Visualizer/ -lVisualizer
INCLUDEPATH += $$PWD/../Visualizer
DEPENDPATH += $$PWD/../Visualizer

LIBS += -lboost_python3
LIBS += -lpython3.6m

INCLUDEPATH += /usr/include/python3.6m
