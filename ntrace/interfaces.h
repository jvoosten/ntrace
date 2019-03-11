#pragma once

#include <list>
#include <memory>

/* Input/output interfaces for NTrace */

#include "message.h"
#include "ntrace_exports.h"

namespace NTrace
{

class IManager;

/**
\brief Some suggested log levels.

These are the same as syslog levels. Note that 'Error' level is not related to the
error() log function below.
*/
enum Level
{
  Emergency,
  Alert,
  Critical,
  Error,
  Warning,
  Notice,    ///< Regular notices of program running
  Info,      ///< Additional information (not logged by default)
  Debug      ///< Low level debug messages
};


/**
\brief Interface for NTracer input

An IInput interface provides a log message to a (the) central log message manager. It can
directly produce log messages or capture from some other source.
*/

class NTRACE_EXPORT IInput
{
public:
  // Obligatory empty virtual destructor
  virtual ~IInput () {};

  virtual IManager * NTRACE_CALL getManager () const = 0;
  virtual std::string NTRACE_CALL getName () const = 0;

#if defined(__GCC__) && (__GCC__ >= 4)
  virtual void NTRACE_CALL log (int level, const char *fmt, ...) __attribute__ ((format (printf, 3, 4))) = 0;
  virtual void NTRACE_CALL error (const char *fmt, ...) __attribute__ ((format (printf, 2, 3))) = 0;
  virtual void NTRACE_CALL out (const char *fmt, ...) __attribute__ ((format (printf, 2, 3))) = 0;
#else
  virtual void NTRACE_CALL log (int level, const char *fmt, ...) = 0;
  virtual void NTRACE_CALL error (const char *fmt, ...) = 0;
  virtual void NTRACE_CALL out (const char *fmt, ...) = 0;
#endif
  virtual void NTRACE_CALL log (int level, const std::string &msg) = 0;
  virtual void NTRACE_CALL error (const std::string &msg) = 0;
  virtual void NTRACE_CALL out (const std::string &msg) = 0;

protected:
  /// Protected constructor; you must supply the manager object to it.
  IInput (IManager *mgr)
    : m_manager (mgr)
  {}

  /// The manager object
  IManager *m_manager;
};

/**
\brief Interface for NTracer output

An IOutput interface takes a log message and does something useful with it. It can
either be a filter, writing to a file, store it in a database, printing to stdout, et cetera.
IOutputs are also responsible for formatting the output (apart from the message,
which is preformatted at the moment of creation).

*/
class NTRACE_EXPORT IOutput
{
public:
  // Obligatory empty virtual destructor
  virtual ~IOutput () {};


  /**
  \brief Return the name of the output module
  \return A string

  Each output object has a name (more specifically, each type of output module).
  This function returns the name to distinguish between the various types.
  */
  virtual std::string NTRACE_CALL getName () const = 0;

  /**
  \brief Primary message output method
  \param msg The log message to store

  This function takes a log message and should process it, like storing it on some
  permanent media like disk or a database. However, it could also be a
  filter that filters messages based on module name, log level, etc; or a splitter that
  re-distributes the message into multiple distinations; or a viewer that shows messages in
  a window.
  */
  virtual void NTRACE_CALL saveMessage (const Message &msg) = 0;

protected:
  /**
  \brief Constructor for IOutput
  \param start_time First timestamp registered for manager (for relative timestamps)

  Only initializes the variables.
  */
  IOutput (const Timestamp &start_time)
    : m_startTime (start_time)
  {
  };

  /// First timestamap of manager
  const Timestamp m_startTime;
};


/**
\brief Trace manager

Keeps track of input and output interfaces. Queues log messages from IInputs
and sends to IOutputs. Uses threads to separate logging from actual writing.

*/

class NTRACE_EXPORT IManager
{
public:

  /**
  \brief Create instance of the real manager class

  \note Although this is inside the IManager interface, it will return an instance
  of the real object. The interface is purely to separate implementation from DLL/so exports.
  */
  static IManager * NTRACE_CALL instance () ;
  static void NTRACE_CALL destroy ();


  virtual IInput * NTRACE_CALL registerModule (const std::string &module_name, int initial_log_level = NTrace::Notice) = 0;

  /**
  \brief Return list of current output modules
  \return List of pointers
  */

  virtual std::list<std::weak_ptr<IOutput>> NTRACE_CALL getOutputs () = 0;

  /**
  \brief Add output object to manager.
  */
  virtual void NTRACE_CALL addOutput (IOutput *out) = 0;

  /**
  \brief Remove output object
  \param out The object to remove

  This will remove the object from the internal list of output objects. The object
  is not destroyed!
  */
  virtual void NTRACE_CALL removeOutput (IOutput *out) = 0;

  /**
  \brief Primary message input function
  \param msg Message to process

  This function will take the given message and queus it for distribute to the outputs. It
  may re-order the message if there are other messages waiting in the output
  queue and the timestamp of this message is before any of the other messages' timestamp.
  */
  virtual void NTRACE_CALL pushMessage (const Message &msg) = 0;

  /**
  \brief Create default debug output stream

  Creates an output module that outputs messages to the default output streams:
  # stdout and stderr on Linux
  # OutputDebugString, std::cout and std::cerr on Windows
  */
  virtual void NTRACE_CALL enableDebugOutput () = 0;

protected:
  virtual ~IManager () {};
};

} // namespace

