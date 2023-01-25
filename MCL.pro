QT       += core gui  winextras network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH+= C:/OpenCV4.1.1/MinGW7.3-x64/include
LIBS+=C:/OpenCV4.1.1/MinGW7.3-x64/x64/mingw/bin/libopencv_*.dll

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CmdADB.cpp \
    config.cpp \
    cv.cpp \
    main.cpp \
    mainwindow.cpp \
    mythread.cpp \
    processwindows.cpp \
    testthresh.cpp

HEADERS += \
    CmdADB.h \
    config.h \
    cv.h \
    mainwindow.h \
    mythread.h \
    processwindows.h \
    testthresh.h

FORMS += \
    config.ui \
    mainwindow.ui \
    processwindows.ui \
    testthresh.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
