#pragma once

#include <string>
#include <vector>
#include "Optional.h"

std::string JoinVectorOfStrings(std::vector<std::string> Strings, std::string Delimiter);
std::string ReplaceAllInString(std::string Destination, std::string OldContent, std::string NewContent);
std::vector<std::string> Tokenize(std::string ToTokenize, std::vector<std::string> KeptDelimiters, std::vector<std::string> DiscardedDelimiters);

// Maps a filepath to the contents of the text file found at that filepath
Optional<std::string> ReadContentsOfFile(std::string Filepath);

// Maps a filepath to an "equivalent" legal C++ identifier
// E.g. /home/joebloggs/downloads -> home_joebloggs_downloads
// /home/.config -> home_DOT_config
std::string FilepathToLegalIdentifier(std::string Filepath);

// Returns true if successful, false otherwise.
bool WriteStringIntoFileOverwriting(std::string Filepath, std::string Content);