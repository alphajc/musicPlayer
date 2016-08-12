#-------------------------------------------------
#
# Project created by QtCreator 2016-03-28T16:40:00
#
#-------------------------------------------------

QT       += core gui multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayer
TEMPLATE = app
RC_FILE = logo.rc

SOURCES += main.cpp\
	playerwidget.cpp\
	switchwidget.cpp \
    lyricform.cpp \
    assestutil.cpp \
    rolllabel.cpp \
    lyriclabel.cpp \
    playlist.cpp \
    logindialog.cpp \
    regdialog.cpp \
    userdatebase.cpp \
    tdialog.cpp

HEADERS  += playerwidget.h\
	switchwidget.h \
    lyricform.h \
    assestutil.h \
    rolllabel.h \
    lyriclabel.h \
    playlist.h \
    logindialog.h \
    regdialog.h \
    userdatebase.h \
    tdialog.h

FORMS    += playerwidget.ui \
    lyricform.ui \
    logindialog.ui \
    regdialog.ui \
    tdialog.ui

RESOURCES += \
    resource.qrc

DISTFILES +=

