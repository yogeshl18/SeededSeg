#-------------------------------------------------
#
# Project created by QtCreator 2016-01-10T06:12:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeededSeg
TEMPLATE = app
# add open CV
#unix {
#    CONFIG += link_pkgconfig
 #   PKGCONFIG += opencv
#}
INCLUDEPATH += C:\opencv\buildcmake\bin
INCLUDEPATH += C:\opencv\buildcmake\install\include
INCLUDEPATH +=C:\Yogesh\SeededSeg\SeededSeg\Eigen
LIBS += -L"C:\opencv\buildcmake\bin"
LIBS += -lopencv_core2411d -lopencv_highgui2411d -lopencv_imgproc2411d
SOURCES += main.cpp\
        mainwindow.cpp \
    cimageweights.cpp

HEADERS  += mainwindow.h \
    cimageweights.h

FORMS    += mainwindow.ui
