#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

class SearchEngine {
public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count);

private:
    std::vector<std::string_view> text_;
    std::map<std::string, std::set<size_t>> idf_;
    std::map<size_t, std::map<std::string, long double>> tf_;

    std::vector<std::string_view> MakeList(std::string_view text);

    std::vector<std::string> MakeListQuery(std::string_view text) const;
    std::vector<std::string_view> MakeLineList(const std::string_view &text);
    std::string Lower(const std::string_view &str) const;
};
