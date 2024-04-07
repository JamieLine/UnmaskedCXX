#include "TestCreationStatus.h"

#include <algorithm>

template <typename T>
auto AreAllOK(std::vector<WithStatus<T>> Statuses) -> bool {
  return std::all_of(Statuses.begin(), Statuses.end(),
                     [](WithStatus<T> Status) -> bool {
                       return Status.Status == TestCreationStatus::ALL_OK;
                     });
}