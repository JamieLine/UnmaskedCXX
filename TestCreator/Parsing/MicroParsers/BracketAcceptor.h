#ifndef TESTCREATOR_MICROPARSERS_BRACKETACCEPTOR_H
#define TESTCREATOR_MICROPARSERS_BRACKETACCEPTOR_H

#include <map>
#include <stack>
#include <string>
#include <utility>

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

// Brackets are weird
// A closing bracket is legal if and only if it matches the type of the last
// bracket AND all opened brackets are closed by the end of the section This
// class lets you handle that case
enum BracketType {
  ROUNDED,  // ( )
  SQUARE,   // [ ]
  BRACE,    // { }
  ANGLED,   // < >
};

enum BracketHalf {
  OPENING,
  CLOSING,
};

class BracketAcceptor {
 public:
  static auto AcceptOpeningBracket(TokenArray::RawTokenIterator& FirstToken,
                                   BracketType Type) -> bool;
  static auto AcceptClosingBracket(TokenArray::RawTokenIterator& FirstToken,
                                   BracketType Type) -> bool;

  static auto GetBracketDepth() -> std::size_t;

  BracketAcceptor() = delete;

  static std::vector<BracketType> CurrentlyOpenBrackets;

  static auto GetClosingBrackets(BracketType Type) -> std::string;
  static auto GetOpeningBrackets(BracketType Type) -> std::string;
};

#endif /* TESTCREATOR_MICROPARSERS_BRACKETACCEPTOR_H */
