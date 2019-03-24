ACLOCAL_AMFLAGS=-Im4

if EXAMPLES
SUBDIRS = . examples
else
SUBDIRS = .
endif


lib_LTLIBRARIES=libntrace.la

# Versioning CURRENT:REVISION:AGE
libntrace_la_LDFLAGS=-version-info 7:0:0
	
libntrace_la_SOURCES=\
  ntrace/function.cpp ntrace/manager.cpp ntrace/message.cpp \
  ntrace/output_base.cpp ntrace/timestamp.cpp \
  inputs/module.cpp \
  outputs/debug_output.cpp outputs/file_output.cpp


include_HEADERS=\
  ntrace.h 

nobase_include_HEADERS=\
  ntrace/interfaces.h ntrace/ntrace_exports.h \
  ntrace/function.h ntrace/manager.h inputs/module.h \
  outputs/debug_output.h outputs/file_output.h
