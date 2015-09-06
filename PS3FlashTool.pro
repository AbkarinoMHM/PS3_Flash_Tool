#-------------------------------------------------
#
# Project created by QtCreator 2015-05-14T00:07:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PS3_Flash_Tool

TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            ps3nand.cpp \
            qcustomlineedit.cpp \
            misc.cpp \
    about.cpp

HEADERS  += mainwindow.h \
            ps3nand.h \
            qcustomlineedit.h \
            misc.h \
    about.h

FORMS    += \
            mainwindow.ui \
    about.ui

RESOURCES += \
            ps3flashtool.qrc \
            ps3flashtool_patch_470.qrc \
            ps3flashtool_355.qrc

#DISTFILES += \
#    res/ps3flashtool.rc

win32:RC_FILE = ps3flashtool.rc

