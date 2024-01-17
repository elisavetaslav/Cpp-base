#include "unixpath.h"

#include <vector>

void MakeStackCur(std::vector<std::string> &pas_as_stack, std::string_view path) {
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
}

void MakeStack(std::vector<std::string> &pas_as_stack, std::vector<std::string> &cur_stack, std::string_view path) {
    std::string current_dir;
    for (const char &c : path) {
        if (c == '/') {
            if (current_dir == "..") {
                if (!pas_as_stack.empty()) {
                    pas_as_stack.pop_back();
                }
                if (!cur_stack.empty()) {
                    cur_stack.pop_back();
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
        if (!cur_stack.empty()) {
            cur_stack.pop_back();
        }
    } else if (current_dir != "." && !current_dir.empty()) {
        pas_as_stack.push_back(current_dir);
    }
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::string result;
    if (path[0] == '/') {
        std::vector<std::string> stack;
        MakeStackCur(stack, path);
        for (const std::string &it : stack) {
            if (!it.empty()) {
                result += "/" + it;
            }
        }
        if (result.empty()) {
            result = "/";
        }
    } else {
        std::vector<std::string> stack_cur;
        std::vector<std::string> stack;
        MakeStackCur(stack_cur, current_working_dir);
        MakeStack(stack, stack_cur, path);
        for (const std::string &it : stack_cur) {
            if (!it.empty()) {
                result += "/" + it;
            }
        }
        for (const std::string &it : stack) {
            if (!it.empty()) {
                result += "/" + it;
            }
        }
        if (result.empty()) {
            result = "/";
        }
    }
    return result;
}
