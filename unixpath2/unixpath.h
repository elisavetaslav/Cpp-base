#pragma once

#include <string>
#include <string_view>
#include <vector>

class UnixPath {
public:
    explicit UnixPath(std::string_view initial_dir);

    void ChangeDirectory(std::string_view path);

    std::string GetAbsolutePath() const;
    std::string GetRelativePath() const;

private:
    std::vector<std::string> initial_dir_;
    std::vector<std::string> changed_;

    std::vector<std::string> MakeStack(const std::string_view &path);

    std::vector<std::string> MakeStackPath(const std::string_view &path);
};
