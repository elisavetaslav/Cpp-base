#include "word2vec.h"

#include <algorithm>
#include <tuple>
#include <vector>

int64_t ScalarPr(const std::vector<int>& first, const std::vector<int>& second) {
    int64_t scalar_pr_uv = 0;
    for (size_t i = 0; i < first.size(); ++i) {
        scalar_pr_uv += static_cast<int64_t>(first[i]) * second[i];
    }
    return scalar_pr_uv;
}

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    std::vector<std::tuple<int64_t, size_t>> cosine;
    for (size_t k = 1; k < vectors.size(); ++k) {
        int64_t scalar_pr_uv = ScalarPr(vectors[k], vectors[0]);
        cosine.push_back(std::make_tuple(scalar_pr_uv, k));
    }
    std::stable_sort(cosine.begin(), cosine.end(),
                     [](const std::tuple<int64_t, size_t>& first, const std::tuple<int64_t, size_t>& second) {
                         return (std::get<0>(first) > std::get<0>(second));
                     });
    std::vector<std::string> result;
    if (!cosine.empty()) {
        result.push_back(words[std::get<1>(cosine[0])]);
    }
    for (size_t i = 1; i < cosine.size(); ++i) {
        if (std::get<0>(cosine[0]) == std::get<0>(cosine[i])) {
            result.push_back(words[std::get<1>(cosine[i])]);
        } else {
            break;
        }
    }
    return result;
}
