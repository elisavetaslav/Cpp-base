#include "search.h"

std::vector<std::string_view> SearchEngine::MakeList(std::string_view text) {
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

std::vector<std::string> SearchEngine::MakeListQuery(std::string_view text) const {
    std::vector<std::string> res;
    std::set<std::string> result;
    size_t beg = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (!std::isalpha(text[i])) {
            std::string_view word = text.substr(beg, i - beg);
            if (!word.empty()) {
                result.insert(Lower(word));
            }
            beg = i + 1;
        }
    }
    std::string_view word = text.substr(beg, beg - text.size());
    if (!word.empty()) {
        result.insert(Lower(word));
    }
    for (const std::string &it : result) {
        res.push_back(it);
    }
    return res;
}

std::vector<std::string_view> SearchEngine::MakeLineList(const std::string_view &text) {
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

std::string SearchEngine::Lower(const std::string_view &str) const {
    std::string result;
    for (char i : str) {
        result += static_cast<char>(tolower(i));
    }
    return result;
};

void SearchEngine::BuildIndex(std::string_view text) {
    text_ = MakeLineList(text);
    std::vector<std::vector<std::string_view>> table;
    for (std::string_view it : text_) {
        table.emplace_back(MakeList(it));
    }
    for (size_t i = 0; i < table.size(); ++i) {
        for (size_t j = 0; j < table[i].size(); ++j) {
            tf_[i][Lower(table[i][j])] += 1.0 / static_cast<long double>(table[i].size());
            idf_[Lower(table[i][j])].insert(i);
        }
    }
};

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) {
    std::vector<std::string> query_list = MakeListQuery(query);
    std::vector<std::tuple<long double, size_t>> answer;
    std::vector<std::string_view> result;
    for (size_t i = 0; i < text_.size(); ++i) {
        long double sumi = 0;
        for (size_t k = 0; k < query_list.size(); ++k) {
            if (tf_.count(i) && idf_.count(query_list[k])) {
                if (tf_.at(i).count(query_list[k])) {
                    sumi += tf_.at(i).at(query_list[k]) *
                            static_cast<long double>(std::log(text_.size() / idf_.at(Lower(query_list[k])).size()));
                }
            }
        }
        answer.emplace_back(std::tie(sumi, i));
    }
    std::stable_sort(answer.begin(), answer.end(), [](auto first, auto second) {
        return (std::tie(std::get<0>(first), std::get<1>(second)) > std::tie(std::get<0>(second), std::get<1>(first)));
    });
    for (size_t i = 0; i < answer.size() && results_count > 0; ++i) {
        if (std::get<0>(answer[i]) > 0) {
            result.emplace_back(text_[std::get<1>(answer[i])]);
            --results_count;
        } else {
            break;
        }
    }
    return result;
};