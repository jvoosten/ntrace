#pragma once


#include "../interfaces.h"
#include "../output_base.h"

namespace NTrace
{


/**
\brief Write message to default debug output

Formats string nicely, with process id, relative time and indented function calls.

getName() returns the fixed string "ntrace.debug_output".

*/
class DebugOutput: public OutputBase
{
public:
  DebugOutput ();
  DebugOutput (const Timestamp &start_time);
  ~DebugOutput ();

  virtual void NTRACE_CALL saveMessage (const Message &msg);

private:
  int m_indent;
  std::string m_indentString;
  std::string m_prefix;
};

}