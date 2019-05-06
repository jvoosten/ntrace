#pragma once

#include <cstdint>

#include "ntrace_exports.h"

namespace NTrace
{
/**
\brief Represent timestamp with microsecond precision

This class represents a timestamp as seconds and microseconds in UTC; the microsecond
precision depends on the underlying operating system of course.

It also performs simple arithmetic to compare and subtract timestamps.
*/
class NTRACE_EXPORT Timestamp
{
public:
  Timestamp ();
  Timestamp (uint32_t time, uint32_t micro);

  uint32_t NTRACE_CALL getTime () const;
  uint32_t NTRACE_CALL getMicros () const;
  uint32_t NTRACE_CALL getMillis () const;

  double NTRACE_CALL getFraction () const;
  double NTRACE_CALL getDifference (const Timestamp &since) const;

  // operator overloads.
  Timestamp & NTRACE_CALL operator =(const Timestamp &src);
  bool NTRACE_CALL operator ==(const Timestamp &eq);
  bool NTRACE_CALL operator <(const Timestamp &lt);

private:
  uint32_t m_time;
  uint32_t m_micro;
};


} // namespace