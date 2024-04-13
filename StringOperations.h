#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "Logging.h"

inline auto ReplaceAllInString(std::string ContainingString,
                               const std::string& OldContent,
                               const std::string& NewContent) -> std::string {
  auto Index = ContainingString.find(OldContent);

  while (Index != std::string::npos) {
    ContainingString.replace(Index, OldContent.size(), NewContent);
    Index = ContainingString.find(OldContent);
  }

  return ContainingString;
}

inline auto JoinVectorOfStrings(std::vector<std::string> Strings,
                                const std::string& Delimiter) -> std::string {
  if (Strings.empty()) {
    return "";
  }

  return std::accumulate(std::next(Strings.begin()), Strings.end(), Strings[0],
                         [Delimiter](const std::string& Prefix,
                                     const std::string& Suffix) -> std::string {
                           return Prefix + Delimiter + Suffix;
                         });
}

inline auto IndentEveryLine(const std::string& Content,
                            std::size_t AddedIndentLevel) -> std::string {
  const std::string IndentMarker = std::string(AddedIndentLevel, '\t') + "\n";

  return ReplaceAllInString(
      Content, "\n",
      IndentMarker);  // NOLINT(readability-suspicious-call-argument)
}

#endif /* STRINGOPERATIONS_H */
