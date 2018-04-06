#pragma once

enum class ParseFailure {
    FileReadError = 1,
    InvalidFileFormat = 2,
    FileDoesNotExist = 3
};
