CONFIG += c++11
QMAKE_CXXFLAGS += -Wno-unused -Wno-sign-compare

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

win32{
    QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
}

HEADERS += \
    rapidxml/rapidxml.hpp \
    rapidxml/rapidxml_iterators.hpp \
    rapidxml/rapidxml_print.hpp \
    rapidxml/rapidxml_utils.hpp \
    baseavoidobst.h \
    baselogger.h \
    cell.h \
    config.h \
    configbuilder.h \
    configxmlreader.h \
    deeplogger.h \
    gl_const.h \
    grb.h \
    iavoidobst.h \
    ilogger.h \
    isearch.h \
    loggerfactory.h \
    logxmlreader.h \
    map.h \
    mapbuilder.h \
    mapxmlreader.h \
    mission.h \
    node.h \
    searchresult.h \
    utils.h \
    xmlreader.h \
    xmlwriter.h

SOURCES += \
    baseavoidobst.cpp \
    baselogger.cpp \
    configxmlreader.cpp \
    deeplogger.cpp \
    grb.cpp \
    logxmlreader.cpp \
    main.cpp \
    mapxmlreader.cpp \
    mission.cpp \
    node.cpp \
    utils.cpp \
    xmlreader.cpp \
    xmlwriter.cpp
