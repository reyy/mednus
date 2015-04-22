#-------------------------------------------------
#
# Project created by QtCreator 2015-02-13T15:18:58
#
#-------------------------------------------------

QT       += testlib
QT += multimediawidgets
QT += core gui widgets

#TARGET += tst_mednusnetworktester
TARGET = tst_mednusquiztester
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cpp \
    tst_mednusquiztester.cpp \
    tst_mednusnetworktester.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
INCLUDEPATH += ../mednus_code/
HEADERS += ../mednus_code/MedNUSNetwork.h \
    tst_mednusquiztester.h \
    tst_mednusnetworktester.h
HEADERS += ../mednus_code/MedNUSQuiz.h \
    ../mednus_code/MedNUSQuizQuestion.h

SOURCES += ../mednus_code/MedNUSNetwork.cpp
SOURCES += ../mednus_code/MedNUSQuiz.cpp \
    ../mednus_code/MedNUSQuizQuestion.cpp

OTHER_FILES += \
    ssl/bluebell.d1.comp.nus.edu.sg.crt
