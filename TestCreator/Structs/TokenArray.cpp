#include "TokenArray.h"

#include <algorithm>

#include "TestCreator/Structs/RichToken.h"
#include "VectorOperations.h"

TokenArray::TokenArray(std::string ToTokenize, std::vector<char> KeptDelimiters,
                       std::vector<char> DiscardedDelimiters) {
  // TODO: TEST ALL OF THIS
  //

  RichTokens = std::vector<RichToken>();
  RawTokens = std::vector<std::string>();

  bool PushedTokenThisLoop = false;
  /*const std::vector<char> WhitespaceChars = {
      ' ',
      '\t',
      '\n',
  };*/

  std::size_t CurrentLineNumber = 1;
  std::size_t CurrentStartingColumn = 1;
  std::size_t CurrentInvestigatedColumn = 1;

  std::vector<char> CurrentToken;
  CurrentToken.reserve(512);
  // This number is arbitrary, but we wish for it to be larger than
  // any token ideally. 512 * sizeof(char) = 512B which is trivial on
  // modern desktop hardware.

  for (auto CurrentIter = ToTokenize.begin(); CurrentIter != ToTokenize.end();
       CurrentIter++) {
    const char CurrentChar = *CurrentIter;

    if (VectorContainsChar(KeptDelimiters, CurrentChar)) {
      std::string TokenText = AsString(CurrentToken);

      auto TokenPair = MakeRichRawTokenPair(TokenText, CurrentLineNumber,
                                            CurrentStartingColumn);

      RichTokens.push_back(TokenPair.first);
      RawTokens.push_back(TokenPair.second);

      std::string DelimiterAsToken = std::string(1, CurrentChar);
      CurrentStartingColumn = CurrentInvestigatedColumn;

      auto DelimiterTokenPair = MakeRichRawTokenPair(
          DelimiterAsToken, CurrentLineNumber, CurrentStartingColumn);
      CurrentStartingColumn++;

      // TODO: This should probably be a function and maybe even the ONLY way to
      // push onto these vectors
      RichTokens.push_back(DelimiterTokenPair.first);
      RawTokens.push_back(DelimiterTokenPair.second);

      PushedTokenThisLoop = true;
    }

    else if ((VectorContainsChar(DiscardedDelimiters, CurrentChar)) ||
             (std::isspace(static_cast<unsigned char>(CurrentChar)) != 0)) {
      // If we have multiple discarded delimiters or whitespace in a row, place
      // all of them onto the previous RichToken and let the RawTokens ignore it
      if (CurrentToken.empty()) {
        // This prevents a segfault from trying to access the back of an empty
        // vector which does occur in practice

        // But in that case, we know we're on the first token and can (for now)
        // ignore leading whitespace
        //
        // TODO: CAN WE JUST DO NOTHING?
        if (!RichTokens.empty()) {
          // std::cout << RichTokens.size() << std::endl;
          // auto BackToken = RichTokens.back();
          // std::cout << "IGNORE ME" << std::endl;
          // auto BackText = RichTokens.back().Text;
          // RichTokens.back().Text.push_back('A');
          RichTokens.back().Text.push_back(CurrentChar);

          PushedTokenThisLoop = true;
        }
      }

      else {
        std::string RawTokenText = AsString(CurrentToken);
        std::string RichTokenText = AsString(CurrentToken) + CurrentChar;

        RichTokens.emplace_back(RichTokenText, CurrentLineNumber,
                                CurrentStartingColumn);
        RawTokens.push_back(RawTokenText);

        PushedTokenThisLoop = true;

        CurrentStartingColumn = CurrentInvestigatedColumn;
      }
    }

    // If this can be a comment
    //
    // TODO: The RichToken vector needs to hold comments as well, but somehow
    // let you have iterate nicely in lockstep with the Raw tokens
    // Maybe by doing the whitespace trick, and writing comments onto the end of
    // the previous RichToken and if the comment is at the start then somehow
    // holding it aside until you have another token to glue it to the start of
    else if (CurrentChar == '/') {
      if (*(CurrentIter + 1) == '*') {
        // We read in "/*", read until the opposite

        CurrentIter += 2;  // Read past the beginning of the comment

        while (!(*CurrentIter == '*' && *(CurrentIter + 1) == '/')) {
          CurrentIter++;
        }

        CurrentIter += 2;  // Read past the end of the comment
      }

      else if (*(CurrentIter + 1) == '/') {
        // We read in "//", read until a newline
        while (*CurrentIter != '\n') {
          CurrentIter++;
          //          CurrentStartingColumn = 0;
          //          CurrentLineNumber++;
        }
      }

      else {
        // It's not a comment, it's something else
        CurrentToken.push_back(CurrentChar);
      }
    }

    else {
      CurrentToken.push_back(CurrentChar);
    }

    if (PushedTokenThisLoop) {
      CurrentToken.clear();
      CurrentStartingColumn++;
      PushedTokenThisLoop = false;
    }

    CurrentInvestigatedColumn++;

    if (CurrentChar == '\n') {
      CurrentLineNumber++;
      CurrentInvestigatedColumn = 0;
      CurrentStartingColumn = 0;
    }
  }

  // Some combinations of Kept and Discarded Delimiters can cause empty strings
  // to show up in the final token array. Neither Raw nor Rich wants them

  RawTokens.erase(std::remove_if(RawTokens.begin(), RawTokens.end(),
                                 [](std::string R) { return R.empty(); }),
                  RawTokens.end());

  RichTokens.erase(std::remove_if(RichTokens.begin(), RichTokens.end(),
                                  [](RichToken R) { return R.Text.empty(); }),
                   RichTokens.end());
}

void TokenArray::DEBUG_OutputBothArrays() {
  for (auto RawIter = RawBegin(); RawIter != RawEnd(); RawIter++) {
    std::cout << *RawIter << std::endl;
    std::cout << "@ ";
    std::cout << std::to_string(RawIter.Index);
    std::cout << std::endl;
  }

  std::cout << "\n\n\n\n\n" << std::endl;

  for (auto RichIter = RichBegin(); RichIter != RichEnd(); RichIter++) {
    std::cout << (*RichIter).Text << std::endl;
    std::cout << (*RichIter).LineNumber << std::endl;
    std::cout << (*RichIter).StartColumn << std::endl;
    std::cout << "@ ";
    std::cout << std::to_string(RichIter.Index);
    std::cout << std::endl;
  }
}
