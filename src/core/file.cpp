#include "file.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "../common.h"

namespace explore {

namespace file {

// reads the entire file into a byte vector. Returns true on success
bool read_all(const std::filesystem::path &path, std::vector<u8> &out) {
    std::ifstream file(path, std::ios::binary);
    ASSERT_RET_MSG(file.is_open(), false, "Failed to open file: %ls",
                   path.c_str());

    file.seekg(0, std::ios::end);
    const std::streamsize size = file.tellg();
    ASSERT_RET_MSG(size >= 0, false, "Failed to get file size: %ls",
                   path.c_str());

    file.seekg(0, std::ios::beg);
    ASSERT_RET_MSG(!file.fail(), false,
                   "Failed to seek to beginning of file: %ls", path.c_str());

    out.resize(static_cast<size_t>(size));
    file.read(reinterpret_cast<char *>(out.data()), size);
    const std::streamsize bytesRead = file.gcount();

    if (bytesRead < size) {
        spdlog::warn("expected {} bytes but read {} from file: {}",
                     static_cast<size_t>(size), static_cast<size_t>(bytesRead),
                     path.string());
    }

    out.resize(static_cast<size_t>(bytesRead));
    return bytesRead > 0;
}

// reads the entire file into a std::string. Returns true on success
bool read_all(const std::filesystem::path &path, std::string &out) {
    std::vector<u8> buffer;
    if (!read_all(path, buffer)) {
        return false;
    }
    out.assign(reinterpret_cast<const char *>(buffer.data()), buffer.size());
    return true;
}

}  // namespace file

namespace path {

// constructs a filesystem path from an initializer list of path segments
std::filesystem::path construct(
    const std::initializer_list<std::string_view> parts) {
    std::filesystem::path result;
    for (auto &part : parts) {
        result /= part;
    }
    return result;
}

}  // namespace path
}  // namespace explore
