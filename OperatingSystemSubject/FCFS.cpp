#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

struct Process {
    int process_id;
    int arrival_time;
    int computing_time;
    int finish_time;
};


void fcfsScheduling(queue<Process>& processes) {
    int currentTime = 0;
    vector<Process> resultProcesses;

    while (!processes.empty()) {
        Process currentProcess = processes.front();
        processes.pop();

        if (currentTime < currentProcess.arrival_time) {
            currentTime = currentProcess.arrival_time;
        }

        currentProcess.finish_time = currentTime + currentProcess.computing_time;
        currentTime = currentProcess.finish_time;

        resultProcesses.push_back(currentProcess);
    }

    // 결과 출력
    cout << "\n\nFCFS Algorithm\n";
    cout << "Process_id\tArrival_time\tComputing_time\tTurnaround_time\tNormalized Turnaround_time\n";
    for (const auto& p : resultProcesses) {
        int turnaround_time = p.finish_time - p.arrival_time;
        cout << p.process_id << "\t\t" << p.arrival_time << "\t\t" << p.computing_time << "\t\t" << turnaround_time << "\t\t" << float(turnaround_time) / float(p.computing_time) << '\n';
    }
}

int main() {
    queue<Process> processes;
    int type, process_id, arrival_time, computing_time;

    cout << "Type\tProcess_ID\tArrival_time\tComputing_time\n";
    while (true) {

        cin >> type;
        if (type == -1) break;

        if (type == 0) {
            cin >> process_id >> arrival_time >> computing_time;
            Process newProcess = { process_id, arrival_time, computing_time, 0 };
            processes.push(newProcess);
        }
        else if (type == 1) {
            // FCFS 알고리즘에선 시간 할당량이 필요없으므로, 입력만 받고 넘기기로 한다.
            cin >> process_id >> arrival_time >> computing_time;
        }
    }

    fcfsScheduling(processes);
    return 0;
}