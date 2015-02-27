#-------------------------------------------------
#
# Project created by QtCreator 2015-02-13T15:18:58
#
#-------------------------------------------------

QT       += testlib
QT += multimediawidgets
QT += core gui widgets

TARGET = tst_mednusnetworktester
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_mednusnetworktester.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
INCLUDEPATH += ../mednus_code/
HEADERS += ../mednus_code/MedNUSNetwork.h

SOURCES += ../mednus_code/MedNUSNetwork.cpp

RESOURCES += \
    Res.qrc

OTHER_FILES += \
    ssl/bluebell.d1.comp.nus.edu.sg.crt
