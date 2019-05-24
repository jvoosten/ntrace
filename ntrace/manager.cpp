#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#include <sys/types.h>

#include <chrono>

#include "manager.h"
#include "inputs/module.h"
#include "outputs/debug_output.h"

/**
\mainpage notitle

\section intro_sec Introduction

 This part of the documentation deals with the internal workings of NTrace.
 If you want to know how to integrate this library into your program read
 the documentation in the doc/ directory (two steps above these generated files).

 The library itself is not very large and most external visible parts are
 exported using interfaces:

 - \ref NTrace::IInput for generating log messages
 - \ref NTrace::IModule for grouping files together in modules
 - \ref NTrace::IOutput for writing log messages somewhere
 - \ref NTrace::IManager that binds it all together

 If you want to extend NTrace for your own purposes you should derive your
 classes from these interfaces (most commonly IInput and IOuput).

 */



using namespace NTrace;

static Manager *s_traceManager = nullptr;

/***************************************************************************/


Manager::Manager ()
{
  m_endLoop = false;
}

Manager::~Manager ()
{
  // Stop thread
  stop ();
  // Clean up modules (should clear IModules through shared pointers)
  m_modules.clear ();
  // output modules are also shared_ptr so automatically cleaned up
  m_outputs.clear ();
}




// public

// Return object instance

IManager *IManager::instance ()
{
  if (s_traceManager == nullptr)
  {
    s_traceManager = new Manager ();
  }
  return s_traceManager;
}

// Perform orderly cleanup

void IManager::shutdown ()
{
  delete s_traceManager;
  s_traceManager = 0;
}

/**
\brief Return initial timestamp of tracing
 */
Timestamp Manager::getStartTimestamp () const
{
  return m_startTime;
}

/**
\brief Get/create a new module
\param module_name Module name; unique per program
\param initial_log_level


Creates a new module or returns an existing one with the same name. The modulename
is a string you supply to identify the module; if multiple files belong to the same
module use the same name (case sensitive). 
 */
IModule *Manager::registerModule (const std::string &module_name, int initial_log_level)
{
  Module *mod = 0;
  modules_list::iterator mit;

  std::lock_guard<std::mutex> lock (m_modulesMutex);
  mit = m_modules.find (module_name);
  if (mit == m_modules.end ())
  {
    mod = new Module (this, module_name, initial_log_level);
    m_modules[module_name] = mod;
  }
  else
  {
    mod = (*mit).second;
  }
  return mod;
}

/**
\brief Return list of all modules
 */
std::list<IModule *> Manager::getModules ()
{
  std::list<IModule *> ret;
  // make copy of the list
  std::lock_guard<std::mutex> lock (m_modulesMutex);
  for (modules_list::iterator it = m_modules.begin (); it != m_modules.end (); ++it)
  {
    ret.push_back (it->second);
  }
  return ret;
}

IModule *Manager::findModule (const std::string &name) const
{
  IModule *ret = nullptr;

  modules_list::const_iterator it = m_modules.find (name);
  if (it != m_modules.end ())
  {
    ret = it->second;
  }

  return ret;
}

std::list<std::weak_ptr<IOutput>> Manager::getOutputs ()
{
  std::list<std::weak_ptr < IOutput>> ret;
  // Make copy of the list
  std::lock_guard<std::mutex> lock (m_outputsMutex);
  for (std::list<output_ptr>::iterator it = m_outputs.begin (); it != m_outputs.end (); ++it)
  {
    ret.push_back (*it);
  }
  return ret;
}

void Manager::addOutput (IOutput *out)
{
  std::shared_ptr<IOutput> ptr;
  ptr.reset (out);
  std::lock_guard<std::mutex> lock (m_outputsMutex);
  m_outputs.push_back (ptr);
  start (); // start output loop if not already busy
}

void Manager::removeOutput (IOutput *out)
{
  std::lock_guard<std::mutex> lock (m_outputsMutex);
  std::list<output_ptr>::iterator it = m_outputs.begin ();
  while (it != m_outputs.end ())
  {
    if ((*it).get () == out)
    {
      it = m_outputs.erase (it);
    }
    else
    {
      ++it;
    }
  }
}

/**
\brief Push message to list
 */
void Manager::pushMessage (const Message &msg)
{
  // Just a quick lock
  m_messagesMutex.lock ();
  m_messages.push_back (msg);
  // Check if our queue gets too big; prune old messages
  if (m_messages.size () > 1000)
  {
    m_messages.pop_front ();
  }
  m_messagesMutex.unlock ();
  // Wake up any waiting output
  m_messagesAvailable.notify_all ();
}

void Manager::enableDebugOutput ()
{
  // Create 
  addOutput (new DebugOutput (m_startTime));
}

/**
\brief Start output thread

 */
void Manager::start ()
{
  if (!m_outputThread.joinable ())
  {
    m_endLoop = false;
    m_outputThread = std::thread (&Manager::outputLoop, this);
  }
}

/**
\brief End output thread

Waits for the thread to finish.
 */

void Manager::stop ()
{
  if (m_outputThread.joinable () && !m_endLoop)
  {
    m_endLoop = true;
    // trigger lock
    m_messagesAvailable.notify_all ();
    m_outputThread.join ();
  }
}

/**
\brief Background thread to write messages
 */
void Manager::outputLoop ()
{
  std::unique_lock<std::mutex> lock (m_messagesMutex);
  while (!m_endLoop)
  {
    // Wait until there are messages available.
    m_messagesAvailable.wait (lock);

    // Do not allow manipulation of outputs while we are processing messages
    m_outputsMutex.lock ();
    while (!m_messages.empty ())
    {
      Message msg = m_messages.front ();
      m_messages.pop_front ();
      // Unlock the messages queue for writing 
      m_messagesMutex.unlock ();

      // We have our message, we can now (slowly) process it
      for (std::list<output_ptr>::iterator it = m_outputs.begin (); it != m_outputs.end (); ++it)
      {
        (*it)->saveMessage (msg);
      }
      // Re-lock because condition_variable expects that
      m_messagesMutex.lock ();
    }
    m_outputsMutex.unlock ();
  }
}


#if 0

/**
 \brief Set output logstream
 \param str Stream object

 Sets the output logstream; this can be cout/cerr, a file or something else
 that is compatible with an ostream. Overrules setLogStream below.
 */
void Manager::setLogStream (std::ostream *str)
{
  if (m_ownLogStream && m_logStream)
  {
    delete m_logStream;
  }
  m_ownLogStream = false;
  m_logStream = str;
}

/**
 \brief Set output log filename
 \param filename

 Filename; the manager creates a stream object to write to. Overrules
 setLogStream above.
 */
void Manager::setLogStream (const std::string &filename)
{
  if (m_ownLogStream && m_logStream)
  {
    delete m_logStream;
  }
  m_ownLogStream = true;
  m_logStream = new std::ofstream (filename.c_str ());
}

/// Enable/disable logging of process ID

void Manager::setLogPid (bool b)
{
  m_logPid = b;
}

/// Enable/disable logging of elapsed time

void Manager::setLogTime (bool b)
{
  m_logTime = b;
}

/// Enable/disable normal logging output to stdout in addition to a log stream

void Manager::setLogToStdout (bool b)
{
  m_logToStdout = b;
}


/// A regular log message

void Manager::log (const std::string &log_string)
{
  std::string ns;

  if (m_mute)
    return;

  ns = makePrefix () + m_indentString + log_string;
  m_loggedText.push_back (ns);
  if (m_loggedText.size () >= m_maxLines)
  {
    m_loggedText.pop_front ();
  }

  if (0 != m_logStream)
  {
    *m_logStream << ns << std::endl;
  }
  if (m_logToStdout)
  {
    std::cout << ns << std::endl;
  }
}

void Manager::error (const std::string &err_string)
{
  std::string ns;

  ns = "!" + makePrefix () + m_indentString + err_string;
  m_loggedText.push_back (ns);
  if (m_loggedText.size () >= m_maxLines)
  {
    m_loggedText.pop_front ();
  }
  if (0 != m_logStream)
  {
    *m_logStream << ns << std::endl;
  }
  std::cerr << ns << std::endl;
}

/**
 \brief Output string to standard output
 \param out_string String to display

 Prints a message directly to stdout, including a prefix. A new line ending
 is automatically appended.
 */
void Manager::out (const std::string &out_string)
{
  std::cout << makePrefix () << out_string << std::endl;
}
#endif

/**
  \brief Read configuration from stream
  \param str Input stream

 */
void Manager::readConfiguration (std::istream &str)
{
  std::string modname;
  int level;
  modules_list::iterator mit;

  if (!str.good ())
  {
    return;
  }
  while (!str.eof ())
  {
    str >> modname >> level;

#if 0
    if ("_logPid" == modname)
    {
      m_logPid = (level > 0);
    }
    else if ("_logTime" == modname)
    {
      m_logTime = (level > 0);
    }
    else
#endif
    {
      mit = m_modules.find (modname);
      if (mit != m_modules.end ())
      {
        (*mit).second->setLevel (level);
      }
    }
  }
}

/**
 \brief Read configuration from filename
 */

void Manager::readConfiguration (const std::string &filename)
{
  std::fstream tracer;
  tracer.open (filename.c_str (), std::fstream::in);
  readConfiguration (tracer);
  tracer.close ();
}

/**
  \brief Write configuration to stream
  \param str output stream
  \sa readConfiguration


 */
void Manager::writeConfiguration (std::ostream &str)
{
  modules_list::const_iterator mit;
  const Module *mod = 0;

  if (!str.good ())
  {
    return;
  }
  for (mit = m_modules.begin (); mit != m_modules.end (); ++mit)
  {
    mod = (*mit).second;
    str << mod->getName () << " " << mod->getLevel () << std::endl;
  }
  //str << "_logPid" << " " << (int) (m_logPid ? 1 : 0) << std::endl;
  //str << "_logTime" << " " << (int) (m_logTime ? 1 : 0) << std::endl;
}

void Manager::writeConfiguration (const std::string &filename)
{
  std::fstream tracer;
  tracer.open (filename.c_str (), std::fstream::out | std::fstream::trunc);
  writeConfiguration (tracer);
  tracer.close ();
}

