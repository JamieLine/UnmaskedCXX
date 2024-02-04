#include "Filepath.h"

#include <fstream>

#include "StringOperations.h"
#include "TestCreator/Structs/LegalIdentifier.h"

auto Filepath::ToLegalIdentifier() const -> LegalIdentifier {
  std::string ToReturn = Path;

  ToReturn = ReplaceAllInString(ToReturn, "./", "");
  ToReturn = ReplaceAllInString(ToReturn, "..", "DOT_DOT");
  ToReturn = ReplaceAllInString(ToReturn, ".", "DOT");
  ToReturn = ReplaceAllInString(ToReturn, "/", "_");
  ToReturn = ReplaceAllInString(ToReturn, "\\", "_");

  return LegalIdentifier(ToReturn);
}

auto Filepath::ReadContentsOfFile() const -> Optional<std::string> {
  std::ifstream InputFilestream;
  InputFilestream.open(Path);

  if (!InputFilestream.is_open()) {
    return {"", false};
  }

  std::string Data((std::istreambuf_iterator<char>(InputFilestream)),
                   (std::istreambuf_iterator<char>()));

  return {Data, true};
}

auto Filepath::WriteStringIntoFileOverwriting(const std::string& Content) const
    -> bool {
  std::ofstream File;
  File.open(Path,
            std::ofstream::in | std::ofstream::out | std::ofstream::trunc);

  if (!File.is_open()) {
    return false;
  }

  File << Content;
  File.close();

  return true;
}
