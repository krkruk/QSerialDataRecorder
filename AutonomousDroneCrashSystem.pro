#-------------------------------------------------
#
# Project created by QtCreator 2016-02-19T14:08:11
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = AutonomousDroneCrashSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    timeintervaldialog.cpp

HEADERS  += mainwindow.h \
    timeintervaldialog.h

FORMS    += mainwindow.ui \
    timeintervaldialog.ui
