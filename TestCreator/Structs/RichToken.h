#ifndef TESTCREATOR_STRUCTS_RICKTOKEN_H
#define TESTCREATOR_STRUCTS_RICKTOKEN_H

#include <string>

struct RichToken {
  std::string Text;
  std::size_t LineNumber;
  std::size_t StartColumn;

  RichToken(std::string InText, std::size_t InLineNumber,
            std::size_t InStartColumn)
      : Text(InText), LineNumber(InLineNumber), StartColumn(InStartColumn) {}
};

inline std::pair<RichToken, std::string> MakeRichRawTokenPair(
    std::string Text, std::size_t LineNumber, std::size_t StartColumn) {
  const RichToken ToReturnRich(Text, LineNumber, StartColumn);
  const std::string ToReturnRaw = Text;
  return std::make_pair(ToReturnRich, ToReturnRaw);
}

#endif /* TESTCREATOR_STRUCTS_RICKTOKEN_H */
