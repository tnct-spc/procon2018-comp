TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT += core gui widgets

CONFIG += precompile_header
PRECOMPILED_HEADER = $$PWD/../precompile/precompile.h
precompile_header:!isEmpty(PRECOMPILED_HEADER) {
    DEFINES += USING_PCH
}
INCLUDEPATH += $$PWD/../precompile
DEPENDPATH += $$PWD/../precompile

SOURCES += \
        main.cpp \
        genetic.cpp \
        simulatedannealing.cpp \
        doubleagentgenetic.cpp

LIBS += -L/usr/lib `pkg-config --libs opencv`
LIBS += -L/usr/lib64 -lzbar

LIBS += -L$$OUT_PWD/../Field/ -lField
INCLUDEPATH += $$PWD/../Field
DEPENDPATH += $$PWD/../Field

LIBS += -L$$OUT_PWD/../Visualizer/ -lVisualizer
INCLUDEPATH += $$PWD/../Visualizer
DEPENDPATH += $$PWD/../Visualizer

LIBS += -L$$OUT_PWD/../Solver/ -lSolver
INCLUDEPATH += $$PWD/../Solver
DEPENDPATH += $$PWD/../Solver

LIBS += -L$$OUT_PWD/../QRcode/ -lQRcode
INCLUDEPATH += $$PWD/../QRcode
DEPENDPATH += $$PWD/../QRcode

HEADERS += \
    genetic.h \
    simulatedannealing.h \
    doubleagentgenetic.h
