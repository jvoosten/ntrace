#pragma once


#include "../interfaces.h"
#include "../output_base.h"

namespace NTrace
{


/**
\brief Output channel to write message to the default debug output

Formats string nicely, with process id, relative time and indented function calls.

getName() returns the fixed string "ntrace.debug_output".

*/
class DebugOutput: public OutputBase
{
public:
  /**
   \brief Constructor for DebugOutput channel with starting timestamp
   \param start_time First timestamp

   The time in the output will be relative to the starting time.
   */
  DebugOutput (const Timestamp &start_time);

  virtual void NTRACE_CALL saveMessage (const Message &msg);

private:
  /**
   \brief Current indentation level
   For each NTrace::Entry message the indentation level is increased, and decreased
   with every NTrace::Exit message. This should provide a nice graphical
   view of the fuction flow.
   */
  int m_indent; ///< Current indentation level
  std::string m_indentString; ///< Pre-calculated indentation string
};

}