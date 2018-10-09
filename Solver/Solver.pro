#-------------------------------------------------
#
# Project created by QtCreator 2018-04-05T17:18:23
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Solver
TEMPLATE = lib

DEFINES += SOLVER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# CONFIG += precompile_header
# PRECOMPILED_HEADER = $$PWD/../precompile/precompile.h
# precompile_header:!isEmpty(PRECOMPILED_HEADER) {
#     DEFINES += USING_PCH
# }
# INCLUDEPATH += $$PWD/../precompile
# DEPENDPATH += $$PWD/../precompile

SOURCES += \
    BreadthFirstSearch/beamsearch.cpp \
        dummyalgorithm.cpp \
    geneticalgo/geneticalgo.cpp \
    geneticalgo/geneticagent.cpp \
    simplemontecarlo/montecarlowithalgo.cpp \
    gamemanager.cpp \
    gamesimulator.cpp \
    algorithmwrapper.cpp \
    geneticalgo/simplealgorithm.cpp \
    doubleagent/agents/agentwrapper.cpp \
    doubleagent/agentmanager.cpp \
    doubleagent/agents/testdoubleagentalgo.cpp \
    doubleagent/agents/evaluateparam.cpp \
    doubleagent/algorithms/simplemcforduble.cpp \
    doubleagent/algorithms/nashequilibrium.cpp \
    useabstractdata.cpp \
    doubleagent/agents/directionbasedalgo.cpp \
    simplemontecarlo/useabstmontecarlo.cpp \
    LastForce/lastforce.cpp \
    majorityrulewithabstdata.cpp \
    depthfirstsearch.cpp \
    LastForce/lastregion.cpp

HEADERS += \
        algorithm_global.h \ 
        testalgorithm.h \
        algorithmwrapper.h \
        gamemanager.h \
        simplemontecarlo/simplemontecarlo.h \
        gamesimulator.h \
        montecarlotreesearch/montecarlotreesearch.h \
        montecarlotreesearch/searchnode.h \
    BreadthFirstSearch/beamsearch.h \
        dummyalgorithm.h \
    geneticalgo/geneticagent.h \
    simplemontecarlo/montecarlowithalgo.h \
    geneticalgo/geneticagent.h \
    geneticalgo/simplealgorithm.h \
    doubleagent/agents/agentwrapper.h \
    doubleagent/agentmanager.h \
    doubleagent/agents/testdoubleagentalgo.h \
    doubleagent/agents/evaluateparam.h \
    doubleagent/algorithms/simplemcforduble.h \
    doubleagent/algorithms/nashequilibrium.h \
    useabstractdata.h \
    doubleagent/agents/directionbasedalgo.h \
    simplemontecarlo/useabstmontecarlo.h \
    LastForce/lastforce.h \
    majorityrulewithabstdata.h \
    depthfirstsearch.h \
    LastForce/lastregion.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L/usr/lib `pkg-config --libs opencv`
LIBS += -L/usr/lib64 -lzbar

LIBS += -L$$OUT_PWD/../Field/ -lField
INCLUDEPATH += $$PWD/../Field
DEPENDPATH += $$PWD/../Field

LIBS += -L$$OUT_PWD/../Visualizer/ -lVisualizer
INCLUDEPATH += $$PWD/../Visualizer
DEPENDPATH += $$PWD/../Visualizer

LIBS += -L$$OUT_PWD/../QRcode/ -lQRcode
INCLUDEPATH += $$PWD/../QRcode
DEPENDPATH += $$PWD/../QRcode
