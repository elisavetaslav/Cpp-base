#include "scorer.h"

void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    if (table_.count(student_name) && table_[student_name].count(task_name)) {
        table_[student_name][task_name][0] = 0;
    } else {
        table_[student_name][task_name] = {0, 3};
    }
};
void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    if (table_.count(student_name) && table_[student_name].count(task_name)) {
        table_[student_name][task_name][0] = 1;
    } else {
        table_[student_name][task_name] = {1, 3};
    }
};
void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    if (table_.count(student_name) && table_[student_name].count(task_name)) {
        table_[student_name][task_name][1] = 2;
    } else {
        table_[student_name][task_name] = {0, 2};
    }
};
void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    if (table_.count(student_name) && table_[student_name].count(task_name)) {
        table_[student_name][task_name][1] = 3;
    } else {
        table_[student_name][task_name] = {0, 3};
    }
};
void Scorer::Reset() {
    table_.clear();
};

ScoreTable Scorer::GetScoreTable() const {
    ScoreTable result;
    for (const auto& [key, value] : table_) {
        if (!value.empty()) {
            for (const auto& [key_map, value_map] : value) {
                if (value_map[0] == 1 && value_map[1] == 3) {
                    result[key].insert(key_map);
                }
            }
        }
    }
    return result;
};