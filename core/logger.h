#pragma once
#include <sstream>
#include <string>

typedef std::stringstream StringBuffer;

class Logger
{
  static const int maxLines = 47;
  static void updateLog()
  {
    // Count lines in buffer
    std::string countString = logBuffer.str();
    std::string delimiter = "\n";
    size_t pos = 0;
    int lineCount = 0;

    while ((pos = countString.find(delimiter)) != -1) {
      lineCount++;
      countString.erase(0, pos + delimiter.length());
    }

    // Scroll buffer
    if (lineCount >= maxLines) {
      std::string tempBuffer = logBuffer.str();
      tempBuffer.erase(0, tempBuffer.find(delimiter) + delimiter.length());
      logBuffer.str(tempBuffer);
    }
  }

public:
  static StringBuffer logBuffer;
  template <typename T> static void println(T content)
  {
    logBuffer << content << "\n";
    updateLog();
  }

  static void error(std::string errorMessage)
  {
    println(("ERR: " + errorMessage));
  }
};
