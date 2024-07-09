#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int process_id;
    int arrival_time;
    int computing_time;
    int finish_time;
};

// 우선순위 큐의 우선순위 설정을 위한 용도의 구조체
struct CompareProcesses {
    bool operator()(const Process& p1, const Process& p2) {
        // computing_time이 동일할 경우, 도착 시간 우선으로 작동하도록 한다.
        if (p1.computing_time == p2.computing_time) {
            return p1.arrival_time > p2.arrival_time;
        }
        // 아니라면 computing_time이 더 짧은 순서대로...
        return p1.computing_time > p2.computing_time;
    }
};

void spnScheduling(queue<Process>& processes) {
    int currentTime = 0;
    vector<Process> resultProcess;
    priority_queue<Process, vector<Process>, CompareProcesses> readyQueue;

    while (!processes.empty() || !readyQueue.empty()) {
        // readyQueue에 새로운 프로세스 추가
        while (!processes.empty() && processes.front().arrival_time <= currentTime) {
            readyQueue.push(processes.front());
            processes.pop();
        }

        // readyQueue가 비어있다면(준비되어 있는 프로세스가 없다면), 시간을 앞당긴다.
        if (readyQueue.empty()) {
            currentTime = processes.front().arrival_time;
            continue;
        }

        // 가장 짧은 computing_time으로 프로세스를 실행한다.
        Process currentProcess = readyQueue.top();
        readyQueue.pop();

        currentProcess.finish_time = currentTime + currentProcess.computing_time;
        currentTime = currentProcess.finish_time;

        resultProcess.push_back(currentProcess);
    }

    // 결과 출력
    cout << "\n\nSPN Algorithm\n";
    cout << "Process_id\tArrival_time\tComputing_time\tTurnaround_time\tNormalized Turnaround_time\n";
    for (const auto& p : resultProcess) {
        int turnaround_time = p.finish_time - p.arrival_time;
        cout << p.process_id << "\t\t" << p.arrival_time << "\t\t" << p.computing_time << "\t\t" << turnaround_time << "\t\t" << float(turnaround_time) / float(p.computing_time) << '\n';
    }
}

int main() {
    queue<Process> processes;
    int type, process_id, priority, computing_time;

    cout << "Type\tProcess_ID\tArrival_time\tComputing_time\n";
    while (true) {
        cin >> type;
        if (type == -1) break;

        if (type == 0) {
            cin >> process_id >> priority >> computing_time;
            Process newProcess = { process_id, priority, computing_time, 0 };
            processes.push(newProcess);
        }
        else if (type == 1) {
            // SPN 알고리즘에선 시간 할당량이 필요없으므로, 입력만 받고 넘기기로 한다.
            cin >> process_id >> priority >> computing_time;
        }
    }

    spnScheduling(processes);
    return 0;
}