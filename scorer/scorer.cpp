#include "scorer.h"

int EventValue(Event event) {
    if (event.event_type == EventType::CheckFailed) {
        return 0;
    } else if (event.event_type == EventType::CheckSuccess) {
        return 1;
    } else if (event.event_type == EventType::MergeRequestClosed) {
        return 2;
    } else {
        return 3;
    }
}

ScoreTable GetScoredStudents(const Events &events, time_t score_time) {
    ScoreTable result;
    Events events_copy;
    std::map<StudentName, std::map<TaskName, std::vector<EventType>>> answer;
    for (const Event &it : events) {
        events_copy.push_back(it);
    }
    std::stable_sort(events_copy.begin(), events_copy.end(),
                     [](const Event first, const Event second) { return (first.time < second.time); });
    for (size_t i = 0; i < events_copy.size() && events_copy[i].time <= score_time; ++i) {
        if ((EventValue(events_copy[i]) == 0 || EventValue(events_copy[i]) == 1)) {
            if ((answer[events_copy[i].student_name]).count(events_copy[i].task_name)) {
                answer[events_copy[i].student_name][events_copy[i].task_name][0] = events_copy[i].event_type;
            } else {
                answer[events_copy[i].student_name][events_copy[i].task_name] = {events_copy[i].event_type,
                                                                                 EventType::MergeRequestClosed};
            }
        } else {
            if ((answer[events_copy[i].student_name]).count(events_copy[i].task_name)) {
                answer[events_copy[i].student_name][events_copy[i].task_name][1] = events_copy[i].event_type;
            } else {
                answer[events_copy[i].student_name][events_copy[i].task_name] = {EventType::CheckFailed,
                                                                                 events_copy[i].event_type};
            }
        }
    }
    for (const auto &[key, value] : answer) {
        for (const auto &[key_map, value_map] : value) {
            if (value_map[0] == EventType::CheckSuccess && value_map[1] == EventType::MergeRequestClosed) {
                result[key].insert(key_map);
            }
        }
    }
    return result;
}
