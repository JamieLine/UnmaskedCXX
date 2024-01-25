#pragma once

#include <string>
#include <vector>
#include "Optional.h"

std::string JoinVectorOfStrings(std::vector<std::string> Strings, std::string Delimiter);
std::string ReplaceAllInString(std::string Destination, std::string OldContent, std::string NewContent);
std::vector<std::string> Tokenize(std::string ToTokenize, std::vector<std::string> KeptDelimiters, std::vector<std::string> DiscardedDelimiters);
Optional<std::string> FilepathToString(std::string Filepath);