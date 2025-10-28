QT += core gui widgets network
TARGET = MetroRoute
TEMPLATE = app
CONFIG += c++11

SOURCES += \
    main.cpp \
    MetroData.cpp \
    RouteCalculator.cpp \
    MetroMapView.cpp \
    MetroPlannerWindow.cpp \
    Chatbot.cpp \
    SettingsDialog.cpp \
    Visualization.cpp

HEADERS += \
    MetroData.h \
    RouteCalculator.h \
    MetroMapView.h \
    MetroPlannerWindow.h \
    Chatbot.h \
    SettingsDialog.h \
    Visualization.h
