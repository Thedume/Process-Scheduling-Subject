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

// �켱���� ť�� �켱���� ������ ���� �뵵�� ����ü
struct CompareProcesses {
    bool operator()(const Process& p1, const Process& p2) {
        // computing_time�� ������ ���, ���� �ð� �켱���� �۵��ϵ��� �Ѵ�.
        if (p1.computing_time == p2.computing_time) {
            return p1.arrival_time > p2.arrival_time;
        }
        // �ƴ϶�� computing_time�� �� ª�� �������...
        return p1.computing_time > p2.computing_time;
    }
};

void spnScheduling(queue<Process>& processes) {
    int currentTime = 0;
    vector<Process> resultProcess;
    priority_queue<Process, vector<Process>, CompareProcesses> readyQueue;

    while (!processes.empty() || !readyQueue.empty()) {
        // readyQueue�� ���ο� ���μ��� �߰�
        while (!processes.empty() && processes.front().arrival_time <= currentTime) {
            readyQueue.push(processes.front());
            processes.pop();
        }

        // readyQueue�� ����ִٸ�(�غ�Ǿ� �ִ� ���μ����� ���ٸ�), �ð��� �մ���.
        if (readyQueue.empty()) {
            currentTime = processes.front().arrival_time;
            continue;
        }

        // ���� ª�� computing_time���� ���μ����� �����Ѵ�.
        Process currentProcess = readyQueue.top();
        readyQueue.pop();

        currentProcess.finish_time = currentTime + currentProcess.computing_time;
        currentTime = currentProcess.finish_time;

        resultProcess.push_back(currentProcess);
    }

    // ��� ���
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
            // SPN �˰��򿡼� �ð� �Ҵ緮�� �ʿ�����Ƿ�, �Է¸� �ް� �ѱ��� �Ѵ�.
            cin >> process_id >> priority >> computing_time;
        }
    }

    spnScheduling(processes);
    return 0;
}