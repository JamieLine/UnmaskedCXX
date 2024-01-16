#pragma once

#include <ostream>

const std::string LOG = "LOG";
const std::string WARN = "WARN";
const std::string ERROR = "ERROR";
const std::string VALUE_OUTPUT = "VALUE_OUTPUT";

void Log(std::ostream& Output, std::string Level, std::string Message);