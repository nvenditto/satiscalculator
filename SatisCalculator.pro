#-------------------------------------------------
#
# Project created by QtCreator 2020-01-14T18:50:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SatisCalculator
TEMPLATE = app

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \ 
    src/selectproductiondialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/aboutdialog.cpp \
    src/productionmodel.cpp \
    src/recipemodel.cpp \
    src/selectrecipedialog.cpp \
    src/recipedelegate.cpp

HEADERS += \ 
    src/selectproductiondialog.h \
    src/Recipe.h \
    src/mainwindow.h \
    src/recipedb.h \
    src/aboutdialog.h \
    src/productionmodel.h \
    src/recipemodel.h \
    src/selectrecipedialog.h \
    src/recipedelegate.h

FORMS += \
    src/mainwindow.ui \
    src/aboutdialog.ui \
    src/selectproductiondialog.ui \
    src/selectrecipedialog.ui

RESOURCES += \
    resources.qrc \
    src/qdarkstyle/style.qrc
    src/qdarkstyle/style.qrc
