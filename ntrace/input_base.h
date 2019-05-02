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
  virtual IManager *NTRACE_CALL getManager () const;
  virtual std::string NTRACE_CALL getName () const;

protected:
  InputBase (IManager *mgr, const std::string &name);
  ~InputBase ();

private:
  std::string m_name;
};


};
