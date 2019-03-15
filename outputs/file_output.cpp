#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#endif

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdio.h>
#include <time.h>

#include "file_output.h"

using namespace NTrace;

#if defined(_WIN32)
#define DIR_SEPARATOR   '\\'
#elif defined (__GNUC__)
#define DIR_SEPARATOR   '/'
#else
#error System not supported.
#endif


/**
\brief FileOutput constructor
\param basename Full path to the log file basename (without extension)
\param extension String with filename extension (including '.')
\param max_file_size Maximum allowed file size (in bytes); 0 means unlimited size and no rotation.
\param max_number_of_files Maximum number of old log files to keep; default of 0 is unlimited.

Sets up an Output module that writes to a text file; the output is formatted to be human-readable.

\note Opening of the file (and purging) is deferred until the first message is saved to the output.

The current log filename is always the \p basename and \p extension concatenated together; if you
specify a \p max_file_size and the file reaches the limit, the file will be closed, renamed to
basename-YYMMDD-HHMMSS.extension and a new file will be openend. If \p max_number_of_files
is also set, old files will be purged at that moment.

Specifying \p max_number_of_files without a \p max_file_size is possible but pointless.

getName() returns the fixed string "ntrace.file_output".
*/
FileOutput::FileOutput (const std::string &basename, const std::string &extension, unsigned int max_file_size, int max_number_of_files)
  : OutputBase("ntrace.file_output"), 
  m_fileBasename (basename), m_fileExtension (extension), m_maximumFilesize (max_file_size), m_maximumNumberOfFiles (max_number_of_files)
{
  m_currentFileSize = 0;
}


void FileOutput::saveMessage (const Message &msg)
{
  if (!m_outStream.is_open ())
  {
    if (!openOutputStream ())
    {
      return;
    }
  }

  // Format string
  std::ostringstream buf;
  const int timebuf_len = 25;
  char timebuf[timebuf_len] = { '\0' };
  struct tm *when = 0;
  time_t st = 0;

  // Log processs ID
  buf << "(" << std::setw (5) << msg.pid << ") ";

  // Timestamp (absolute)
  st = msg.timestamp.getTime ();
  when = gmtime (&st);
  if (nullptr == when)
  {
    buf << "[??-??-?? ??:??:??.???] ";
  }
  else
  {
    strftime (timebuf, timebuf_len, "%y-%m-%d %H:%M:%S", when);
    buf << "[" << timebuf << ".";
    snprintf (timebuf, timebuf_len, "%03d", msg.timestamp.getMillis ());
    buf << timebuf << "] ";
  }

  // Finish buffer, including line-end
  buf << msg.message << std::endl;

  // Check filesize
  m_currentFileSize += buf.str ().length (); // will be reset in rotateOutputStream
  if (m_maximumFilesize > 0 &&
    m_currentFileSize > m_maximumFilesize)
  {
    if (!rotateOutputStream ())
    {
      // Hmm, oops?
      return;
    }
  }

  // Hmm, do we need std::flush as well?
  m_outStream << buf.str ();
}


/**
  \brief Try to open the log file


*/
bool FileOutput::openOutputStream ()
{
  m_currentFilename = m_fileBasename + m_fileExtension;

  // get file info, see if we need to rotate now.
#if defined(_WIN32)
  HANDLE hf = CreateFile (m_currentFilename.c_str (),
    0, // no access required, just for size.
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL);
  if (INVALID_HANDLE_VALUE == hf)
  {
    // Does not exist, ok
    m_currentFileSize = 0;
  }
  else
  {
    LARGE_INTEGER large;
    GetFileSizeEx (hf, &large);
    m_currentFileSize = large.LowPart;
    CloseHandle (hf);
  }
#else if defined(__GNUC__)
  // use stat
#endif

  m_outStream.open (m_currentFilename, std::ios_base::out | std::ios_base::app | std::ios_base::ate);
  return m_outStream.is_open ();
}


/**
\brief Create new output file and rotate logs

Closes the current output file, renames it, then purges old log files and
reopens the output file.
*/
bool FileOutput::rotateOutputStream ()
{
  std::string new_name;
  const int namebuf_len = 20;
  char namebuf[namebuf_len];

  if (m_outStream.is_open ())
  {
    m_outStream.close ();
  }


  time_t now;
  struct tm *now_tm;
  time (&now);
  now_tm = gmtime (&now);
  if (nullptr == now_tm)
  {
    // Unlikely to happen...
    return false;
  }

  strftime (namebuf, namebuf_len, "%y%m%d-%H%M%S", now_tm);
  new_name = m_fileBasename;
  new_name += namebuf;
  new_name += m_fileExtension;

  if (rename (m_currentFilename.c_str (), new_name.c_str ()))
  {
    return false;
  }

  if (m_maximumNumberOfFiles > 0)
  {
    checkAndPurgeLogfiles ();
  }
  return openOutputStream ();
}


/**
\brief Check for logfiles based on the basename and remove older ones

Note: files are removed based on age, not on name.
*/
void FileOutput::checkAndPurgeLogfiles ()
{
  std::string pattern;
  std::map<time_t, std::string> m_fileTimes;

  pattern = m_fileBasename + "*" + m_fileExtension;

#if defined(_WIN32)
  HANDLE find;
  WIN32_FIND_DATA data;
  ULARGE_INTEGER ui;

  find = FindFirstFile (pattern.c_str (), &data);
  if (INVALID_HANDLE_VALUE == find)
  {
    return;
  }

  do {
    // store timestamp with filename. Convert FILETIME to time_t-ish
    ui.LowPart = data.ftCreationTime.dwLowDateTime;
    ui.HighPart = data.ftCreationTime.dwHighDateTime;
    // Divide by 10^7 to go from 100ns to seconds
    uint64_t us = ui.QuadPart / 10000000ULL;
    // Subtract epoch to reach Jan 1, 1970. 
    us -= 11644473600ULL;
    m_fileTimes[us] = data.cFileName;

  } while (FindNextFile (find, &data));
  FindClose (find);

#elif defined (__GNUC__)

#endif

  // Extract directory part
  size_t slash_pos = m_fileBasename.find_last_of (DIR_SEPARATOR);
  std::string basedir;
  if (std::string::npos != slash_pos)
  {
    basedir = m_fileBasename.substr (0, slash_pos + 1);
  }

  // Files should be stored sorted, accoring to the key (time_t)
  std::map<time_t, std::string>::iterator it = m_fileTimes.begin ();
  while (m_fileTimes.size () > m_maximumNumberOfFiles)
  {
    remove ((basedir + it->second).c_str());
    it = m_fileTimes.erase (it);
  }
}


