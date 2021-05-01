TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    source/graph.cpp \
    source/graph_node.cpp \
    source/database.cpp \
    source/instance.cpp \
    source/router.cpp \
    source/metadata.cpp \
    source/tenant.cpp \
    source/file.cpp \
    source/hash_node.cpp \
    source/hash_list.cpp \
    source/hash_array.cpp \
    source/log.cpp \
    source/cryptography.cpp \
    source/timer.cpp

HEADERS += \
    header/graph.h \
    header/graph_node.h \
    header/database.h \
    header/instance.h \
    header/router.h \
    header/metadata.h \
    header/tenant.h \
    header/file.h \
    header/hash_node.h \
    header/hash_list.h \
    header/hash_array.h \
    header/log.h \
    header/cryptography.h \
    header/timer.h

LIBS += -lpthread
