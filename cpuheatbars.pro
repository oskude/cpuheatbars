QT       += core gui webenginewidgets websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cpuheatbars
TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    procstat.cpp

HEADERS  += \
    server.h \
    procstat.h

DISTFILES += \
    main.html

RESOURCES += \
    main.qrc
