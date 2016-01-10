#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T16:24:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab6
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    loginwindow.cpp \
    signupwindow.cpp \
    rosterwindow.cpp \
    messageswindow.cpp \
    listrosterwidget.cpp \
    rosteritem.cpp \
    searchwindow.cpp \
    clientconnect.cpp \
    threadcheck.cpp

HEADERS  += mainwindow.h \
    loginwindow.h \
    signupwindow.h \
    rosterwindow.h \
    messageswindow.h \
    rosteritem.h \
    listrosterwidget.h \
    searchwindow.h \
    rpcsources/im.h \
    clientconnect.h \
    threadcheck.h

FORMS    += mainwindow.ui \
    loginwindow.ui \
    signupwindow.ui \
    rosterwindow.ui \
    messageswindow.ui \
    searchwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/rpcsources/release/ -lim_client
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/rpcsources/debug/ -lim_client
else:unix: LIBS += -L$$PWD/rpcsources/ -lim_client

INCLUDEPATH += $$PWD/rpcsources
DEPENDPATH += $$PWD/rpcsources

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/rpcsources/release/libim_client.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/rpcsources/debug/libim_client.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/rpcsources/release/im_client.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/rpcsources/debug/im_client.lib
else:unix: PRE_TARGETDEPS += $$PWD/rpcsources/libim_client.a
