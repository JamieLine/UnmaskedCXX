#ifndef TESTCREATOR_STRUCTS_TESTCREATIONSTATUS
#define TESTCREATOR_STRUCTS_TESTCREATIONSTATUS

#include <vector>

enum class TestCreationStatus {
  ALL_OK,
  COULD_NOT_READ_INPUT_FILE,
  COULD_NOT_OPEN_OUTPUT_FILE,
  FOUND_UNEXPECTED_TOKEN_FROM_MAP,
  GIVEN_INVALID_PARSED_RESULT,
  ATTEMPTED_ILLEGAL_PARSE,
};

template <typename T>
struct WithStatus {
  T Item;
  TestCreationStatus Status;

  WithStatus(T InItem, TestCreationStatus InStatus)
      : Item(InItem), Status(InStatus) {}
};

template <typename T>
auto AreAllOK(std::vector<WithStatus<T>> Statuses) -> bool;

#endif /* TESTCREATOR_STRUCTS_TESTCREATIONSTATUS */
