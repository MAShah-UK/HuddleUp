#-------------------------------------------------
#
# Project created by QtCreator 2016-03-17T23:49:47
#
#-------------------------------------------------

QT        += core gui widgets multimedia

SOURCES   += main.cpp \
    Helpers/Math_General.cpp \
    Helpers/Qt_General.cpp \
    UI/MainWindow.cpp \
    UI/WideUI/WideRootWidget.cpp \
    UI/TallUI/TallRootWidget.cpp \
    UI/SlideWidget/SlideWidget.cpp \
    UI/SlideWidget/SW_StyleVariant.cpp \
    UI/CaptionWidget/CaptionWidget.cpp \
    UI/Chatrooms/Chatrooms.cpp

HEADERS   += Helpers/Math_General.h \
    Helpers/Qt_General.h \
    UI/MainWindow.h \
    UI/WideUI/WideRootWidget.h \
    UI/TallUI/TallRootWidget.h \
    UI/SlideWidget/SlideWidget.h \
    UI/SlideWidget/SW_StyleVariant.h \
    UI/CaptionWidget/CaptionWidget.h \
    UI/Chatrooms/Chatrooms.h

RESOURCES += resources.qrc

CONFIG    += c++14
