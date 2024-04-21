#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional> // Include for std::function
#endif

#include "schedwork.h"

using namespace std;

static const Worker_T INVALID_ID = (unsigned int)-1; // Make sure this is declared in the appropriate scope

bool scheduleWorkers(size_t day, const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, vector<int>& shifts, DailySchedule& sched, size_t n, size_t k) {
    if (day == n) {
        return true; // Base case: All days are scheduled
    }

    vector<Worker_T> possibleWorkers;
    for (Worker_T i = 0; i < k; ++i) {
        if (avail[day][i] && shifts[i] < maxShifts) {
            possibleWorkers.push_back(i);
        }
    }

    function<bool(size_t, size_t)> dfs = [&](size_t index, size_t start) {
        if (index == dailyNeed) {
            sched[day] = possibleWorkers; // Assuming this assignment is meant to capture the state at this point
            return scheduleWorkers(day + 1, avail, dailyNeed, maxShifts, shifts, sched, n, k);
        }

        for (size_t i = start; i < possibleWorkers.size(); ++i) {
            Worker_T worker = possibleWorkers[i];
            shifts[worker]++;
            if (dfs(index + 1, i + 1)) {
                return true;
            }
            shifts[worker]--;
        }
        return false;
    };

    return dfs(0, 0);
}

bool schedule(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched) {
    if (avail.empty()) return false;
    
    size_t n = avail.size();
    size_t k = avail[0].size();
    sched.resize(n, vector<Worker_T>(dailyNeed, INVALID_ID));
    vector<int> shifts(k, 0);

    return scheduleWorkers(0, avail, dailyNeed, maxShifts, shifts, sched, n, k);
}
