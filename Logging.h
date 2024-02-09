#ifndef LOGGING_H
#define LOGGING_H

#include <ostream>

const std::string LOG = "LOG";
const std::string WARN = "WARN";
const std::string ERROR = "ERROR";
const std::string VALUE_OUTPUT = "VALUE_OUTPUT";

void Log(std::ostream& Output, const std::string& Level,
         const std::string& Message);

#endif /* LOGGING_H */
