#include "admission.h"

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    AdmissionTable result;
    std::unordered_map<std::string, size_t> max_sts;
    for (const University& university : universities) {
        result[university.name] = std::vector<const Student*>();
        max_sts[university.name] = university.max_students;
    }
    std::vector<const Applicant*> st_table;
    for (auto& applicant : applicants) {
        st_table.emplace_back(&applicant);
    }
    std::stable_sort(st_table.begin(), st_table.end(), [](const Applicant* first, const Applicant* second) {
        return (std::tie((*first).points, (*first).student.birth_date.year, (*first).student.birth_date.month,
                         (*first).student.birth_date.day, (*first).student.name) >
                std::tie((*second).points, (*second).student.birth_date.year, (*second).student.birth_date.month,
                         (*second).student.birth_date.day, (*second).student.name));
    });
    for (const Applicant* applicant : st_table) {
        for (const std::string& university : applicant->wish_list) {
            if (result[university].size() < max_sts[university]) {
                result[university].emplace_back(&applicant->student);
                break;
            }
        }
    }
    return result;
}
