#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <string>
#include <vector>

#include "Optional.h"

auto JoinVectorOfStrings(std::vector<std::string> Strings,
                         const std::string& Delimiter) -> std::string;
auto ReplaceAllInString(std::string Destination, const std::string& OldContent,
                        const std::string& NewContent) -> std::string;
auto Tokenize(const std::string& ToTokenize,
              std::vector<std::string> KeptDelimiters,
              std::vector<std::string> DiscardedDelimiters)
    -> std::vector<std::string>;

auto IndentEveryLine(std::string Content, std::size_t AddedIndentLevel)
    -> std::string;

#endif /* STRINGOPERATIONS_H */
