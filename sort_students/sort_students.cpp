#include "sort_students.h"

bool SortName(const Student& student1, const Student& student2) {
    if (student1.last_name != student2.last_name) {
        return (student1.last_name < student2.last_name);
    } else {
        if (student1.name != student2.name) {
            return (student1.name < student2.name);
        } else {
            if (student1.birth_date.year != student2.birth_date.year) {
                return (student1.birth_date.year > student2.birth_date.year);
            } else {
                if (student1.birth_date.month != student2.birth_date.month) {
                    return (student1.birth_date.month > student2.birth_date.month);
                } else {
                    return (student1.birth_date.day > student2.birth_date.day);
                }
            }
        }
    }
}

bool SortDate(const Student& student1, const Student& student2) {
    if (student1.birth_date.year != student2.birth_date.year) {
        return (student1.birth_date.year < student2.birth_date.year);
    } else {
        if (student1.birth_date.month != student2.birth_date.month) {
            return (student1.birth_date.month < student2.birth_date.month);
        } else {
            if (student1.birth_date.day != student2.birth_date.day) {
                return (student1.birth_date.day < student2.birth_date.day);
            } else {
                if (student1.last_name != student2.last_name) {
                    return (student1.last_name < student2.last_name);
                } else {
                    return (student1.name < student2.name);
                }
            }
        }
    }
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    if (sortKind == SortKind::Date) {
        std::stable_sort(students.begin(), students.end(), SortDate);
    } else {
        std::stable_sort(students.begin(), students.end(), SortName);
    }
}
