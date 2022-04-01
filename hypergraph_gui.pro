QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += mumps5 armadillo

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hypergraph_struct/edge.cpp \
    hypergraph_struct/hypergraph.cpp \
    hypergraph_struct/vertex.cpp \
    main.cpp \
    mainwindow.cpp \
    paintarea.cpp

HEADERS += \
    hypergraph_struct/edge.h \
    hypergraph_struct/hypergraph.h \
    hypergraph_struct/type.h \
    hypergraph_struct/vertex.h \
    mainwindow.h \
    paintarea.h

FORMS += \
    mainwindow.ui \
    paintarea.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
