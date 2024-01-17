#include "tests_checking.h"

#include <deque>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<std::string> students;
    std::vector<std::string> result;
    for (const StudentAction& student : student_actions) {
        if (student.side == Side::Top) {
            students.push_front(student.name);
        } else {
            students.push_back(student.name);
        }
    }
    for (const size_t& i : queries) {
        result.emplace_back(students[i - 1]);
    }
    return result;
}
