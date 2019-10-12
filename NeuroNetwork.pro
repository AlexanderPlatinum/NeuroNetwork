QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NeuroNetwork
TEMPLATE = app


SOURCES += ./src/main.cpp\
           ./src/MainWindow.cpp \
           ./src/ProjectWriter.cpp \
           ./src/ProjectReader.cpp \
           ./src/Network.cpp

HEADERS  += ./include/MainWindow.h \
            ./include/ProjectStructure.h \
            ./include/ProjectSymbol.h \
            ./include/ProjectWriter.h \
            ./include/ProjectReader.h \
            ./include/Network.h

INCLUDEPATH += ./include

FORMS    += ./ui/MainWindow.ui

RESOURCES += \
             resource.qrc
