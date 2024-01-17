#include "unixpath.h"

std::vector<std::string> UnixPath::MakeStack(const std::string_view &path) {
    std::vector<std::string> pas_as_stack;
    std::string current_dir;
    for (const char &c : path) {
        if (c == '/') {
            pas_as_stack.push_back(current_dir);
            current_dir = "";
        } else {
            current_dir += c;
        }
    }
    if (!current_dir.empty() && current_dir != "/") {
        pas_as_stack.push_back(current_dir);
    }
    return pas_as_stack;
}

std::vector<std::string> UnixPath::MakeStackPath(const std::string_view &path) {
    std::string current_dir;
    std::vector<std::string> pas_as_stack = changed_;
    for (const char &c : path) {
        if (c == '/') {
            if (current_dir == "..") {
                if (!pas_as_stack.empty()) {
                    pas_as_stack.pop_back();
                }
            } else if (current_dir != "." && !current_dir.empty()) {
                pas_as_stack.push_back(current_dir);
            }
            current_dir = "";
        } else {
            current_dir += c;
        }
    }
    if (current_dir == "..") {
        if (!pas_as_stack.empty()) {
            pas_as_stack.pop_back();
        }
    } else if (current_dir != "." && !current_dir.empty()) {
        pas_as_stack.push_back(current_dir);
    }
    return pas_as_stack;
}

UnixPath::UnixPath(std::string_view initial_dir) {
    initial_dir_ = MakeStack(initial_dir);
    changed_ = initial_dir_;
};

void UnixPath::ChangeDirectory(std::string_view path) {
    if (path[0] == '/') {
        changed_ = MakeStack(path);
    } else {
        changed_ = MakeStackPath(path);
    }
};

std::string UnixPath::GetAbsolutePath() const {
    std::string result;
    for (const std::string &it : changed_) {
        if (!it.empty()) {
            result += "/" + it;
        }
    }
    if (result.empty()) {
        result = "/";
    }
    return result;
};
std::string UnixPath::GetRelativePath() const {
    std::string result;
    size_t index = 0;
    while (index < std::min(initial_dir_.size(), changed_.size())) {
        if (initial_dir_[index] == changed_[index]) {
            ++index;
        } else {
            break;
        }
    }
    if (index == initial_dir_.size()) {
        result = ".";
    } else if (index != 0) {
        for (size_t i = 0; i < initial_dir_.size() - index; ++i) {
            result += "/..";
        }
    }
    for (size_t it = index; it < changed_.size(); ++it) {
        if (!changed_[it].empty()) {
            result += "/" + changed_[it];
        }
    }
    if (!result.empty()) {
        if (result[0] == '/') {
            result.erase(0, 1);
        }
    } else {
        result = "/";
    }
    return result;
};
