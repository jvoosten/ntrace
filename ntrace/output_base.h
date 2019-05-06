#pragma once

#include "interfaces.h"

namespace NTrace
{

/**
  \brief Some common/handy functions for output objects.

  */
class OutputBase : public IOutput
{
public:
  virtual std::string NTRACE_CALL getName () const;

protected:
  /**
   \brief Constructor without starting time
   \param name The output channel name/type

   The name is unique per output channel.
   */
  OutputBase (const std::string &name);
  
  /**
   \brief Constructor with starting time
   \param name The ouput channel name/type
   \param start_time Starting timestamp of program (not the creation of the output channel)

   Overloaded constructor that also has the starting timestamp (which is conventien to print
   relative timestamps) */
  OutputBase (const std::string &name, const Timestamp &start_time);

  /// The starting timestamp
  const Timestamp m_startTime;

private:
  /// The name of our output channel
  std::string m_name;
};


}