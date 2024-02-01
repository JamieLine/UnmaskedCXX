#pragma once

#include <string>

enum TestCreationStatus {
    ALL_OK,
    COULD_NOT_READ_INPUT_FILE,
    COULD_NOT_OPEN_OUTPUT_FILE,
    FOUND_UNEXPECTED_TOKEN_FROM_MAP,
};

TestCreationStatus CreateTestsFromFile(std::string FilePath);