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
  ntrace/input_base.cpp ntrace/output_base.cpp ntrace/timestamp.cpp \
  ntrace/inputs/module.cpp \
  ntrace/outputs/debug_output.cpp ntrace/outputs/file_output.cpp


include_HEADERS=\
  ntrace.h 

nobase_include_HEADERS=\
  ntrace/interfaces.h ntrace/ntrace_exports.h \
  ntrace/function.h ntrace/manager.h ntrace/message.h ntrace/timestamp.h ntrace/input_base.h ntrace/output_base.h \
  ntrace/inputs/module.h \
  ntrace/outputs/debug_output.h ntrace/outputs/file_output.h
