#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Define the worker type and schedule data structures
typedef unsigned int Worker_T;
typedef vector<vector<bool>> AvailabilityMatrix;
typedef vector<vector<Worker_T>> DailySchedule;

static const Worker_T INVALID_ID = (unsigned int)-1;

// Prototypes for helper functions
bool canPlaceWorker(int day, Worker_T worker, const vector<int>& workerShifts, const AvailabilityMatrix& avail, const size_t maxShifts);
bool scheduleDay(int day, vector<int>& workerShifts, const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched);

// Main scheduling function
bool schedule(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched) {
    if(avail.empty() || avail[0].empty()) return false;

    size_t n = avail.size(); // Number of days
    size_t k = avail[0].size(); // Number of workers

    // Initialize the schedule and worker shift count
    sched = DailySchedule(n, vector<Worker_T>(dailyNeed, INVALID_ID));
    vector<int> workerShifts(k, 0);

    // Start recursive scheduling from the first day
    return scheduleDay(0, workerShifts, avail, dailyNeed, maxShifts, sched);
}

// Recursive function to schedule each day
bool scheduleDay(int day, vector<int>& workerShifts, const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched) {
    if (day == sched.size()) {
        return true; // All days scheduled successfully
    }

    vector<Worker_T> &todaySchedule = sched[day];
    size_t k = avail[day].size();

    // Try to place each worker in today's schedule
    function<bool(int)> placeNextWorker = [&](int index) -> bool {
        if (index == dailyNeed) {
            // All needed workers placed for today, move to the next day
            return scheduleDay(day + 1, workerShifts, avail, dailyNeed, maxShifts, sched);
        }

        for (Worker_T worker = 0; worker < k; ++worker) {
            if (canPlaceWorker(day, worker, workerShifts, avail, maxShifts)) {
                // Place the worker
                todaySchedule[index] = worker;
                workerShifts[worker]++;
                
                // Recur to place the next worker
                if (placeNextWorker(index + 1)) {
                    return true;
                }
                
                // Backtrack
                workerShifts[worker]--;
            }
        }
        return false;
    };

    return placeNextWorker(0);
}

// Helper function to check if a worker can be placed on a given day
bool canPlaceWorker(int day, Worker_T worker, const vector<int>& workerShifts, const AvailabilityMatrix& avail, const size_t maxShifts) {
    return avail[day][worker] && workerShifts[worker] < maxShifts;
}

#ifndef RECCHECK
int main() {
    AvailabilityMatrix avail = {
        {true, true, true, true},
        {true, false, true, false},
        {true, true, false, true},
        {true, false, false, true}
    };
    size_t dailyNeed = 2;
    size_t maxShifts = 2;
    DailySchedule sched;

    if (schedule(avail, dailyNeed, maxShifts, sched)) {
        for (const auto& day : sched) {
            for (Worker_T worker : day) {
                cout << worker << " ";
            }
            cout << endl;
        }
    } else {
        cout << "No valid schedule found." << endl;
    }

    return 0;
}
#endif
