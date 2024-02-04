#ifndef TESTCREATOR_STRUCTS_FILEPATH_H
#define TESTCREATOR_STRUCTS_FILEPATH_H

#include <string>
#include <utility>

#include "Optional.h"
#include "TestCreator/Structs/LegalIdentifier.h"

// POD struct, used to leverage strong typing as a form of error checking
// against badly ordered arguments to functions.
struct Filepath {
 public:
  std::string Path;

  explicit Filepath(std::string InPath) : Path(std::move(InPath)) {}

  // Maps a filepath to an "equivalent" legal C++ identifier
  // E.g. /home/joebloggs/downloads -> home_joebloggs_downloads
  // /home/.config -> home_DOT_config
  auto ToLegalIdentifier() const -> LegalIdentifier;

  // Maps a filepath to the contents of the text file found at that filepath
  auto ReadContentsOfFile() const -> Optional<std::string>;

  // Returns true if successful, false otherwise.
  auto WriteStringIntoFileOverwriting(const std::string& Content) const -> bool;

  explicit operator std::string() const { return Path; }
};

#endif /* TESTCREATOR_STRUCTS_FILEPATH_H */
