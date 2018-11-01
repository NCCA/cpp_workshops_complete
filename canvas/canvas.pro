######################################################################
# Automatically generated by qmake (3.0) Mon Mar 17 14:27:06 2014
######################################################################

TEMPLATE = app
TARGET = canvas
INCLUDEPATH += .
LIBS += -lSDL2
QMAKE_CXXFLAGS += -std=c++11
OBJECTS_DIR = obj

# Input
SOURCES += src/main.cpp \
    src/canvas.cpp \
    src/operation.cpp \
    src/lineoperation.cpp \
    src/rectangleoperation.cpp

HEADERS += \
    src/canvas.h \
    src/operation.h \
    src/lineoperation.h \
    src/rectangleoperation.h

OTHER_FILES += \
    README.txt \
    canvas.dox

DISTFILES += $$OTHER_FILES
