#ifdef _WIN32
#include <windows.h>
#endif
#ifdef HAVE_CONFIG_H
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#endif

#include "timestamp.h"

using namespace NTrace;

/**
\brief Timestamp for 'now'

Creates a Timestamp with the current time.

*/
Timestamp::Timestamp ()
{
#ifdef _WIN32
  FILETIME ft;
  ULARGE_INTEGER ui;

  // Tick. 'File time' is some big-ass integer that counts in 100ns intervals since 1601 (Kepler would have died to get such an accurate clock, then)
  ::GetSystemTimeAsFileTime (&ft);
  // Tock. Convert to ULARGE_INTEGER 
  ui.LowPart = ft.dwLowDateTime;
  ui.HighPart = ft.dwHighDateTime;
  // Divide by 10 to go from 100ns to microseconds
  uint64_t us = ui.QuadPart / 10L;
  // Subtract epoch to reach Jan 1, 1970. (found on stackoverflow.com)
  us -= 11644473600000000LL;

  m_micro = us % 1000000;
  m_time = (uint32_t)(us / 1000000);
#endif
#ifdef HAVE_SYS_TIME_H
  struct timeval tv;

  gettimeofday (&tv, 0);
  m_time = tv.tv_sec;
  m_micro = tv.tv_usec;
#endif
}

/**
 \brief Create Timestamp with pre-set time
 \param time Time in seconds since epoch
 \param micro Sub-microsecond part of timestam[p
 */

Timestamp::Timestamp (uint32_t time, uint32_t micro)
{
  m_time = time;
  m_micro = micro;
}


/**
\brief Return seconds part of timestamp
*/

uint32_t Timestamp::getTime () const
{
  return m_time;
}

/**
\brief Return microseconds part of timestamp
*/
uint32_t Timestamp::getMicros () const
{
  return m_micro;
}
/**
\brief Return milliseconds part of timestamp (derived from microseconds)
*/

uint32_t Timestamp::getMillis () const
{
  return m_micro / 1000;
}

/**
\brief Return timestamp as a fractional number.


*/
double Timestamp::getFraction () const
{
  return m_time + m_micro / 1e6;
}

/**
 \brief Calculate difference between two timestamps
 \param since Timestamp to calculate to

 Returns the difference between the current timestamp and \p since as a fractional
 number. If \p since is in the past, returns a positive number.


 */
double Timestamp::getDifference (const Timestamp &since) const
{
  return getFraction () - since.getFraction ();
}


Timestamp &Timestamp::operator =(const Timestamp &src)
{
  m_time = src.m_time;
  m_micro = src.m_micro;
  return *this;
}

bool Timestamp::operator ==(const Timestamp &eq)
{
  return m_time == eq.m_time  && m_micro == eq.m_micro;
}

bool Timestamp::operator <(const Timestamp &lt)
{
  if (m_time < lt.m_time)
  {
    return true;
  }
  if (m_time == lt.m_time && m_micro < lt.m_micro)
  {
    return true;
  }
  return false;
}
