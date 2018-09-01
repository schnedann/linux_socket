TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    tcp_client.cpp \
    mqtt_client.cpp

HEADERS += \
    tcp_client.h \
    mqtt_client.h
