QT       += core gui opengl
include(qtgifimage/src/gifimage/qtgifimage.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets openglwidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../Backend/s21_parser.c \
    ../../Backend/s21_affine_transformations.c \
    glview.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../../Backend/s21_parser.h \
    ../../Backend/s21_affine_transformations.h \
    glview.hpp \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
