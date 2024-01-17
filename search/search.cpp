#include "search.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <set>

bool IsEqual(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(a[i]) != std::tolower(b[i])) {
            return false;
        }
    }
    return true;
}

std::vector<std::string_view> MakeList(std::string_view text) {
    std::vector<std::string_view> res;
    size_t beg = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (!std::isalpha(text[i])) {
            std::string_view word = text.substr(beg, i - beg);
            if (!word.empty()) {
                res.push_back(word);
            }
            beg = i + 1;
        }
    }
    std::string_view word = text.substr(beg, beg - text.size());
    if (!word.empty()) {
        res.push_back(word);
    }
    return res;
}

bool Cmp(const std::string_view& a, const std::string_view& b) {
    if (a.size() < b.size()) {
        return true;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(a[i]) >= std::tolower(b[i])) {
            return false;
        }
    }
    return true;
}

using Comp = std::integral_constant<decltype(&Cmp), &Cmp>;

std::vector<std::string_view> MakeListQuery(std::string_view text) {
    std::vector<std::string_view> res;
    std::set<std::string_view, Comp> result;
    size_t beg = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (!std::isalpha(text[i])) {
            std::string_view word = text.substr(beg, i - beg);
            if (!word.empty()) {
                result.insert(word);
            }
            beg = i + 1;
        }
    }
    std::string_view word = text.substr(beg, beg - text.size());
    if (!word.empty()) {
        result.insert(word);
    }
    for (std::string_view it : result) {
        res.push_back(it);
    }
    return res;
}

std::vector<std::string_view> MakeLineList(std::string_view text) {
    std::vector<std::string_view> res;
    bool was_word = false;
    size_t beg = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (std::isalpha(text[i])) {
            was_word = true;
        } else if (text[i] == '\n') {
            std::string_view line = text.substr(beg, i - beg);
            if (!line.empty() && was_word) {
                res.push_back(line);
            }
            beg = i + 1;
            was_word = false;
        }
    }
    std::string_view line = text.substr(beg, text.size() - beg);
    if (!text.empty() && was_word) {
        res.push_back(line);
    }
    return res;
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::vector<std::string_view> text_v = MakeLineList(text);
    std::vector<std::vector<std::string_view>> table;
    std::vector<std::tuple<long double, size_t>> answer;
    std::vector<std::string_view> result;
    for (std::string_view it : text_v) {
        table.emplace_back(MakeList(it));
    }
    std::vector<std::string_view> query_list = MakeListQuery(query);
    std::map<size_t, int64_t> idf;
    std::map<size_t, std::map<size_t, int64_t>> tf;
    for (size_t i = 0; i < table.size(); ++i) {
        for (size_t k = 0; k < query_list.size(); ++k) {
            bool was_count = false;
            for (size_t j = 0; j < table[i].size(); ++j) {
                if (IsEqual(query_list[k], table[i][j])) {
                    if (tf.count(i)) {
                        if (tf[i].count(k)) {
                            ++tf[i][k];
                        } else {
                            tf[i][k] = 1;
                        }
                    } else {
                        tf[i][k] = 1;
                    }
                    if (idf.count(k) && !was_count) {
                        ++idf[k];
                        was_count = true;
                    } else if (!was_count) {
                        idf[k] = 1;
                        was_count = true;
                    }
                }
            }
        }
    }
    for (size_t i = 0; i < table.size(); ++i) {
        long double sum = 0;
        for (size_t j = 0; j < table[i].size(); ++j) {
            for (size_t k = 0; k < query_list.size(); ++k) {
                if (IsEqual(query_list[k], table[i][j]) && idf[k] != 0 && !table[i].empty()) {
                    sum += tf[i][k] * static_cast<long double>(std::log(table.size() / idf[k])) / table[i].size();
                }
            }
        }
        answer.emplace_back(std::tie(sum, i));
    }
    std::stable_sort(answer.begin(), answer.end(), [](auto first, auto second) {
        return (std::tie(std::get<0>(first), std::get<1>(second)) > std::tie(std::get<0>(second), std::get<1>(first)));
    });
    for (size_t i = 0; i < answer.size() && results_count > 0; ++i) {
        if (std::get<0>(answer[i]) > 0) {
            result.emplace_back(text_v[std::get<1>(answer[i])]);
            --results_count;
        } else {
            break;
        }
    }
    return result;
}
