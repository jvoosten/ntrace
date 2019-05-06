#pragma once

#include "interfaces.h"

namespace NTrace
{

/**
  \brief Some common/handy functions for input objects.

  */

class InputBase: public virtual IInput
{
public:
  virtual std::string NTRACE_CALL getName () const;

protected:
  /**
   \brief Construct IInput object with quick pointer to IManager and a name
   \param mgr Manager object
   \param name Input channel name.

   Stores a pointer to the IManager object (strictly speaking this is not necessary
   but saves a lot of overhead).
   */
  InputBase (IManager *mgr, const std::string &name);

private:
  /// The channel name
  std::string m_name;
};


};
