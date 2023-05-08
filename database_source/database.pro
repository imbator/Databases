QT += core gui
QT += sql
QT += core
LIBS += -LE:/Qt/6.4.3/mingw_64/plugins/sqldrivers -lqsqlpsql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

QMAKE_LFLAGS += -Wl,-rpath,E:/Qt/6.4.3/mingw_64/plugins/sqldrivers
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databaseinterface.cpp \
    databaseservice.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    configuratedatabase.h \
    databaseinterface.h \
    databaseservice.h \
    mainwindow.h

FORMS += \
    databaseinterface.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
