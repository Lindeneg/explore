#ifndef EXPLORE_CORE_FILE_H_
#define EXPLORE_CORE_FILE_H_

#include <filesystem>
#include <initializer_list>

#include "../common.h"

#define FPATH(...) explore::path::construct({__VA_ARGS__})

namespace explore::file {

// reads the entire contents of a file into a dynamic array
bool read_all(const std::filesystem::path &path, std::vector<u8> &out);
// reads the entire contents of a file into a string
bool read_all(const std::filesystem::path &path, std::string &out);

// TODO: allow reading chunks
}  // namespace explore::file

namespace explore::path {
// constructs a path from a list of strings
std::filesystem::path construct(std::initializer_list<std::string_view> paths);
}  // namespace explore::path

#endif  // EXPLORE_CORE_FILE_H_
