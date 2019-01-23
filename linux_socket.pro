TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    tcp_client.cpp \
    mqtt_client.cpp \
    debug_print.cpp

HEADERS += \
    tcp_client.h \
    mqtt_client.h \
    dtypes.h \
    debug_print.h \
    meta.h
