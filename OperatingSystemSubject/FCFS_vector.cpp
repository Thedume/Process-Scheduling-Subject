#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int process_id;
    int priority;
    int arrival_time;
    int cpu_time;
    int finish_time;
};

void fcfsScheduling(vector<Process>& processes) {
    int currentTime = 0;
    vector<Process> finishedProcesses;

    for (auto& currentProcess : processes) {
        if (currentTime < currentProcess.arrival_time) {
            currentTime = currentProcess.arrival_time;
        }

        currentProcess.finish_time = currentTime + currentProcess.cpu_time;
        currentTime = currentProcess.finish_time;

        finishedProcesses.push_back(currentProcess);
    }

    // Output the results
    cout << "Process_id\tPriority\tComputing_time\tTurnaround_time\n";
    for (const auto& p : finishedProcesses) {
        int turnaround_time = p.finish_time - p.arrival_time;
        cout << p.process_id << "\t\t" << p.priority << "\t\t" << p.cpu_time << "\t\t" << turnaround_time << '\n';
    }
}

int main() {
    vector<Process> processes;
    int type, process_id, priority, computing_time;

    while (true) {
        cin >> type;
        if (type == -1) break;

        if (type == 0) {
            cin >> process_id >> priority >> computing_time;
            Process newProcess = { process_id, priority, priority, computing_time, 0 };
            processes.push_back(newProcess);
        }
        else if (type == 1) {
            // Ignore the input as per the given sequence
            cin >> process_id >> priority >> computing_time;
        }
    }

    // Sort processes by arrival time for FCFS scheduling
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
        });

    fcfsScheduling(processes);
    return 0;
}
