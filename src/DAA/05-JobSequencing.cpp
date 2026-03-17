/* Algorithm (Job Sequencing with Deadline - Greedy):
1. Start
2. Define a `Job` struct { id, deadline, profit }.
3. Sort jobs in decreasing order of profit (greedy choice: highest profit first).
4. Find maxDeadline = maximum deadline value across all jobs.
5. Create a schedule array of size maxDeadline+1, initialised to -1 (free).
6. For each job j (in sorted order):
   a. For slot t from min(maxDeadline, j.deadline) down to 1:
      - If schedule[t] == -1 (slot is free):
           Assign j to slot t; break inner loop.
7. Collect all assigned jobs from schedule (t = 1..maxDeadline).
8. Print selected job IDs and total profit.
9. End
*/

#include <algorithm>
#include <iostream>
#include <vector>

#include "src/commons.hpp"

struct Job {
    char id;
    int  deadline;
    int  profit;
};

void jobSequencing(std::vector<Job>& jobs) {
    // Greedy: sort by decreasing profit
    std::sort(jobs.begin(), jobs.end(),
              [](const Job& a, const Job& b) { return a.profit > b.profit; });

    int maxDeadline = 0;
    for (const auto& j : jobs)
        if (j.deadline > maxDeadline) maxDeadline = j.deadline;

    // schedule[t] = index into jobs[] assigned to time slot t (1-indexed)
    std::vector<int> schedule(maxDeadline + 1, -1);

    for (int i = 0; i < (int)jobs.size(); i++) {
        // Find the latest free slot at or before this job's deadline
        for (int t = std::min(maxDeadline, jobs[i].deadline); t >= 1; t--) {
            if (schedule[t] == -1) {
                schedule[t] = i;
                break;
            }
        }
    }

    std::cout << "Scheduled Jobs: ";
    int totalProfit = 0;
    for (int t = 1; t <= maxDeadline; t++) {
        if (schedule[t] != -1) {
            std::cout << jobs[schedule[t]].id << " ";
            totalProfit += jobs[schedule[t]].profit;
        }
    }
    std::cout << "\nTotal Profit:   " << totalProfit << std::endl;
}

int main() {
    Header("JOB SEQUENCING WITH DEADLINE");

    // Each job: { id, deadline, profit }
    std::vector<Job> jobs = {
        { 'A', 2, 100 },
        { 'B', 1,  19 },
        { 'C', 2,  27 },
        { 'D', 1,  25 },
        { 'E', 3,  15 }
    };

    std::cout << "Jobs (id, deadline, profit):\n";
    for (const auto& j : jobs)
        std::cout << "  " << j.id << "  deadline=" << j.deadline
                  << "  profit=" << j.profit << "\n";
    std::cout << std::endl;

    jobSequencing(jobs);

    Footer();
    return 0;
}
