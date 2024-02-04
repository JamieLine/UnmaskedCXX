#ifndef OPTIONAL_H
#define OPTIONAL_H

template <typename T>
struct Optional {
  // If the data is deemed to not exist, then T is in
  // indeterminate state.
  // If construction of T is cheap, it is allowed to take
  // a "sensible default"
  // Otherwise, it is fully permitted to simply either memset
  // T to zeroes, or leave the memory as it is found.
  T Data;
  bool DataExists;

  Optional(const T& InData, bool InDataExists)
      : Data(InData), DataExists(InDataExists) {}
  // cppcheck-suppress uninitMemberVar
  Optional() : DataExists(false) {}  // NOLINT
};

#endif /* OPTIONAL_H */
