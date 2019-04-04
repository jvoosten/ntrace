#pragma once

#include <string>

#include "timestamp.h"

namespace NTrace
{

class IInput;

/**
\brief A single log message

Used internally to represent a single log message with timestamp, module name, messag and log level.
This will be distributed to the output modules which then decides what to do with it.

*/
struct Message
{
public:
  /// Pointer to the module that generated the message
  const IInput *module;
  /// Log level (the higher, the less important); only relevant for 'Normal' messages
  int level;
  /// The type of message
  enum Type {
    Normal,       // Regular log messages
    Out,          // Log message intended for stdout
    Error,        // Important log message, intended for stderr or logging in Release mode
    Entry,        // Function entry messages
    Exit,         // Function exit messages
  } type;
  /// The message
  std::string message;
  /// When the message was generated
  Timestamp timestamp;
  /// Process ID
  int pid;
  /// Thread ID
  int tid;

  Message ();
};

}