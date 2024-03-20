#ifndef WRITINGLOGGING_H
#define WRITINGLOGGING_H

#include <cstddef>
#include <ostream>

class WritingLogging {
 public:
  static void Log(std::ostream& Output, bool IsOkay,
                  const std::string& Message);
  static void OutputValue(std::ostream& Output, const std::string& Value);

  static void IncreaseIndentationLevel();
  static void DecreaseIndentationLevel();

 private:
  static std::size_t IndentationLevel;
};

#endif /* WRITINGLOGGING_H */
