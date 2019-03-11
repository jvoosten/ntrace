#if defined(_WIN32)
#include <windows.h>
#else
// todo: HAVE_CONFIG_H and checks for pthread
#include <pthread.h>
#include <unistd.h>
#endif

#include "message.h"

using namespace NTrace;

/**
\brief Constructor

Sets the timestamp, process and thread ID
*/
Message::Message ()
{
#if defined(_WIN32)
  pid = (int)::GetCurrentProcessId ();
  tid = (int)::GetCurrentThreadId ();
#else
  pthread_t pt;
  pid = getpid ();
  pt = pthread_self ();
  tid = (int)pt;
#endif
}
