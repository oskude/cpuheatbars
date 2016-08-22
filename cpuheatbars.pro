QT       += core gui webenginewidgets websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cpuheatbars
TEMPLATE = app


SOURCES += main.cpp \
    server.cpp

HEADERS  += \
    server.h

DISTFILES += \
    main.html

RESOURCES += \
    main.qrc
