#ifndef TESTCREATOR_STRUCTS_TOKENARRAY_H
#define TESTCREATOR_STRUCTS_TOKENARRAY_H

#include <iostream>
#include <memory>
#include <vector>

#include "RichToken.h"

// typedef std::vector<std::string> TokenArray;

// Forward declaration
class Task;

class TokenArray {
 private:
  std::vector<std::string> RawTokens;
  std::vector<RichToken> RichTokens;

  // TODO: Maybe TokenArray shouldn't also hold its parsing task tree? But
  // that's difficult and probably not worth it?
  Task* RootTask;
  

 public:
  Task* CurrentTask;


  class RichTokenIterator;  // Forward declaration, so we can define a Raw ->
                            // Rich converter. The other direction doesn't need
                            // a forward declaration.

  class RawTokenIterator
      : public std::iterator<std::input_iterator_tag, std::string, std::size_t,
                             const std::string*, std::string&> {
   private:
    friend TokenArray;
    friend RichTokenIterator;
    TokenArray& SourceArray;
    std::size_t Index;
    explicit RawTokenIterator(std::size_t InIndex, TokenArray& Source)
        : Index(InIndex), SourceArray(Source) {}

   public:
    //    explicit RawTokenIterator(TokenArray& Source)
    //        : Index(0), SourceArray(Source) {}

    RawTokenIterator& operator++() {
      Index++;
      return *this;
    }

    // This is the postfix ++ operator
    RawTokenIterator& operator++(int) {
      Index++;
      return *this;
    }

    bool operator==(RawTokenIterator Other) { return Index == Other.Index; }

    bool operator!=(RawTokenIterator Other) { return !(*this == Other); }

    RawTokenIterator operator-(int X) {
      return RawTokenIterator(Index - X, SourceArray);
    }

    reference operator*() const { return SourceArray.RawTokens.at(Index); }

    RichTokenIterator ToRich() { return RichTokenIterator(Index, SourceArray); }

    TokenArray& GetArray() { return SourceArray; }

    Task* GetCurrentTask() { return SourceArray.CurrentTask; }

    // TODO: Write a "PrintAround" method that outputs the tokens around the one
    // this iterator points to
  };

  RawTokenIterator RawBegin() { return RawTokenIterator(0, *this); }
  RawTokenIterator RawEnd() {
    return RawTokenIterator(RawTokens.size(), *this);
  }

  class RichTokenIterator
      : public std::iterator<std::input_iterator_tag, RichToken, std::size_t,
                             const RichToken*, RichToken&> {
   private:
    friend TokenArray;
    friend RawTokenIterator;
    TokenArray& SourceArray;
    std::size_t Index;
    explicit RichTokenIterator(std::size_t InIndex, TokenArray& Source)
        : Index(InIndex), SourceArray(Source) {}

   public:
    RichTokenIterator& operator++() {
      Index++;
      return *this;
    }

    RichTokenIterator& operator++(int) {
      Index++;
      return *this;
    }

    bool operator==(RichTokenIterator Other) { return Index == Other.Index; }
    bool operator!=(RichTokenIterator Other) { return !(*this == Other); }

    reference operator*() const { return SourceArray.RichTokens.at(Index); }

    RawTokenIterator ToRaw() { return RawTokenIterator(Index, SourceArray); }
  };

  RichTokenIterator RichBegin() { return RichTokenIterator(0, *this); }
  RichTokenIterator RichEnd() {
    return RichTokenIterator(RichTokens.size(), *this);
  }

  // TODO: Move Tokenize into here
  TokenArray(std::string TaskDescription, std::string ToTokenize,
             std::vector<char> KeptDelimiters,
             std::vector<char> DiscardedDelimiters);

  void DEBUG_OutputBothArrays();

  inline Task* GetTaskTreeRoot() { return RootTask; }


  ~TokenArray();
};

/*
#include "Task.h"

template <typename ReturnType, typename... ArgumentTypesWithoutToken>
ReturnType PerformSubTask(std::string Description,
                          ReturnType (*F)(TokenArray::RawTokenIterator&,
                                          ArgumentTypesWithoutToken...),
                          TokenArray::RawTokenIterator& FirstToken,
                          ArgumentTypesWithoutToken... Args) {
  TokenArray& ThisTokenArray = FirstToken.GetArray();

  ThisTokenArray.CurrentTask->SubTasks.emplace_back(
      Description, ThisTokenArray.CurrentTask, FirstToken);
  ThisTokenArray.CurrentTask = &ThisTokenArray.CurrentTask->SubTasks.back();

  ReturnType ReturnValue = F(FirstToken, Args...);

  ThisTokenArray.CurrentTask = ThisTokenArray.CurrentTask->Parent;

  return ReturnValue;
} */

// template<typename ReturnType, typename... ArgumentTypesWithoutToken>
// ReturnType PerformSubTask(std::string Description, ReturnType
// (*F)(TokenArray::RawTokenIterator&, ArgumentTypesWithoutToken...),
// TokenArray::RawTokenIterator& FirstToken, ArgumentTypesWithoutToken... Args);

#endif /* TESTCREATOR_STRUCTS_TOKENARRAY_H */
