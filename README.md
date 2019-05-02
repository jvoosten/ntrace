# Introduction

Ntrace is a small library to enable tracing and debugging of your code at the
same time. In other words, it allows you to see which functions
are called in which order, and also print debug information in between.
The output is formatted so that the call trace is easy to follow.

The library allows you to divide your code into modules and set the
debug-level per module, thus only generating output for the parts you are
interested in without having to insert/remove the debugging code all the time.

To minimize impact on performance it uses a separate thread to write the
output. Using macros it is also possible to eliminate most (if not all) debugging
for your release builds.

# Features

* C++ library
* Uses C-style macros for easy integration in your code
* Allows printf() style formatting
* Traces function entry and exit automatically
* Divide your code into modules, set debug level per module
* Outputs can be redirected to multiple outputs, but critical errors are logged to stderr (even in release mode)
* Each log message is timestamped with millisecond precision, process and thread ID
* Thread-safe
* Available for Windows and Linux (other POSIX-like systems should work as well)

# Sample output

Here's a sample of the library output:

    [  85.401]         << void Drawer::setTerminal(const Terminal*)
    [  85.401]       << void System::terminalRemoved(Terminal*)
    [  85.401]       >> void vt::Control::removeProxy(vt::Socket*) (proxy 0x1411ac8)
    [  85.401]         Removed socket from descriptor list
    [  85.401]       << void vt::Control::removeProxy(vt::Socket*)
    [  85.401]     << void vt::Control::remove(Terminal*, bool)
    [  85.401]   << void vt::Control::terminalStopped(Terminal*)
    [ 109.000]   >> virtual Terminal::~Terminal()
    [ 120.309]     >> virtual ZoneDB::~ZoneDB()
    [ 120.310]       >> void ZoneDB::Purge() (195 pagestacks)

# Sample code

Here's a sample of how to integrate Ntrace in your code:

    void Control::removeProxy (vt::Socket *proxy)
    {
      TR_FUNC ("proxy %p", proxy);

      ...

      std::list <struct pollfd>::iterator fdit = m_socketDescriptors.begin ();
      while (fdit != m_socketDescriptors.end ())
      {
        if ((*fdit).fd == fd)
        {
          fdit = m_socketDescriptors.erase (fdit);
          TR (1, "Removed socket from descriptor list");
        }
        else
        {
          ++fdit;
        }
      }
    }

For more information, see the documentation in the doc/ directory.