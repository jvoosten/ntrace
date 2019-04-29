#ifndef NTRACE_H
#define NTRACE_H

#include "ntrace/interfaces.h"
#include "ntrace/function.h"

#include "ntrace/outputs/debug_output.h"
#include "ntrace/outputs/file_output.h"

// Define this macro in your project settings to enable the full set of TR macros.
// This should normally only be done for your debug build

#ifdef ENABLE_NTRACE

  #if defined(__GNUC__)
  #define FUNCNAME __PRETTY_FUNCTION__
  #elif defined(_MSC_VER)
  #if (_MSC_VER >= 1300)
  #define FUNCNAME __FUNCTION__
  #else
  #define FUNCNAME __FILE__,__LINE__
  #endif
  #else
  #error Compiler not supported (neither GNU C or Microsoft Visual Studio)
  #endif

  #define TR_MODULE(name) static NTrace::IInput *s_trace_module = NTrace::IManager::instance()->registerModule(name)

  #define TR_FUNC     NTrace::Function TracerObject(s_trace_module, FUNCNAME); TracerObject
  #define TR          s_trace_module->log
  #define TR_ERR      s_trace_module->error
  #define TR_OUT      s_trace_module->out

#else

  /* Replace NTRACE macros with dummies */
  #define TR_MODULE(name)

  #define TR_FUNC(...)
  #define TR
  #define TR_ERR(...) fprintf (stderr, __VA_ARGS__); fprintf (stderr, "\n")
  #define TR_OUT(...)

#endif

#endif
