TARGET = ntrace
TEMPLATE = lib
CONFIG += staticlib create_prl
CONFIG += debug_and_release debug_and_release_target
CONFIG -= qt

HEADERS += ntrace.h
HEADERS += ntrace/function.h   ntrace/manager.h   ntrace/module.h
SOURCES += ntrace/function.cpp ntrace/manager.cpp ntrace/module.cpp
