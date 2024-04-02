#ifndef TESTINCLUDES_ADVANCEDLOGGING_H
#define TESTINCLUDES_ADVANCEDLOGGING_H

#include <iostream>

class AdvancedLogging {
 public:
  AdvancedLogging(std::string Title)
      : Title("<" + Title + "> "), IndentationLevel(0) {}

  void Log(std::ostream& Output, bool IsOkay, const std::string& Message);
  void OutputValue(std::ostream& Output, const std::string& Value);

  void IncreaseIndentationLevel();
  void DecreaseIndentationLevel();

 protected:
  std::size_t IndentationLevel;
  std::string Title;
};

static AdvancedLogging WritingLogging("WRITER");
static AdvancedLogging TestLogging("TEST");

#endif /* TESTINCLUDES_ADVANCEDLOGGING_H */
