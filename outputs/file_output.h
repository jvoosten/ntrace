#pragma once

#include <fstream>

#include "../ntrace/interfaces.h"
#include "../ntrace/output_base.h"

namespace NTrace
{


/**
\brief Log output to a file

Includes options for maximum file length, automatic recycling of log files, etc.

By default FileOutput writes endlessly to a single log file; however, when you enable
a maximum size or log file rotation, a timestamp gets appended to the filename (of the form YYMMDD-HHMMSS).
In addition you can restrict the number of log files; old logfiles are automatically removed.

*/
class FileOutput : public OutputBase
{
public:
  NTRACE_EXPORT FileOutput (const std::string &basename, const std::string &extension, unsigned int max_file_size = 0, int max_number_of_files = 0);

  virtual void NTRACE_CALL saveMessage (const Message &msg);

private:
  std::string m_fileBasename;
  std::string m_fileExtension;

  unsigned int m_maximumFilesize;
  unsigned int m_maximumNumberOfFiles;

  std::ofstream m_outStream;
  std::string m_currentFilename;
  long m_currentFileSize;

  bool openOutputStream ();
  bool rotateOutputStream ();
  void checkAndPurgeLogfiles ();
};

}
