#ifndef LOGGER_H
#define LOGGER_H

/*
 * NTrace example logger file
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_NTRACE
#include <ntrace.h>

#if defined(__GNUC__)
#define FUNCNAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#if (_MSC_VER >= 1300)
#define FUNCNAME __FUNCDNAME__
#else
#define FUNCNAME __FILE__,__LINE__
#endif
#else
#error Compiler not supported (neither GNU C or Microsoft Visual Studio)
#endif

#define TR_MODULE(name) static NTrace::Module *s_trace_module = NTrace::Manager::instance()->registerModule(name)

#define TR_FUNC     NTrace::Function TracerObject(s_trace_module, FUNCNAME); TracerObject
#define TR          s_trace_module->log
#define TR_ERR      s_trace_module->error

#define TR_READ_CONFIG(stream)  NTrace::Manager::instance()->readConfiguration(stream)
#define TR_WRITE_CONFIG(stream) NTrace::Manager::instance()->writeConfiguration(stream)
#define TR_SET_LOG(stream)      NTrace::Manager::instance()->setLogStream(stream)
#define TR_SHUTDOWN             delete NTrace::Manager::instance

#else
/* Simulate NTRACE macros by dummies */
#define TR_MODULE(name)

#define TR_FUNC(...)
#define TR
#define TR_ERR(...) fprintf (stderr, __VA_ARGS__); fprintf (stderr, "\n");

#define TR_READ_CONFIG(stream)
#define TR_WRITE_CONFIG(stream)
#define TR_SET_LOG(stream)
#define TR_SHUTDOWN

#endif

#endif
