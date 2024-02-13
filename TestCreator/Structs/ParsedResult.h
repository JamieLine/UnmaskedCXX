#ifndef TESTCREATOR_STRUCTS_PARSEDRESULT_H
#define TESTCREATOR_STRUCTS_PARSEDRESULT_H

template <typename ResultType>
struct ParsedResult {
  bool WasLegalInput;
  ResultType Result;
};

#endif /* TESTCREATOR_STRUCTS_PARSEDRESULT_H */
