#-------------------------------------------------
#
# Project created by QtCreator 2018-06-21T16:12:44
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRcode
TEMPLATE = lib

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
       qrcode.cpp \
       qrconverterfield.cpp

HEADERS += \
       qrcode.h
       qrconverterfield.h

FORMS += \
   qrcode.ui

INCLUDEPATH += /usr/include/opencv

LIBS += -L/usr/lib ｀pkg-config --libs opencv｀
LIBS += -L/usr/lib64 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lzbar

LIBS += -L$$OUT_PWD/../Field/ -lField
INCLUDEPATH += $$PWD/../Field
DEPENDPATH += $$PWD/../Field
