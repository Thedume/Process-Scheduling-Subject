#include <iostream>
#include <algorithm>
#include <queue>
#include <stdlib.h>
using namespace std;

// 2024-05-21-01:19 Commit

// ���μ����� ������ ���� ����ü
typedef struct process {
	int number = 0; // ���μ��� ��ȣ
	int arrive_t = 0; // �����ð�
	int burst_t = 0; // ����ð�
	int priority = 0; // �켱����
}process;

// processing time ����� ���� ����ü
typedef struct processing {
	int start;
	int end;
	int burst;
}processing;

// ����ð� ����
struct cmpburst {
	bool operator()(process a, process b) {
		return a.burst_t > b.burst_t;
	}
};

// �켱���� ����
struct cmppriority {
	bool operator()(process a, process b) {
		return a.priority > b.priority;
	}
};

int process_num; // ���μ��� ����
int quantum = 1; // quantum
process* list; // �� ���μ����� ������ ����Ʈ
vector<processing>* processing_time; // processingtime �迭
int* burst_time; // bursttime �迭
int* complete_time; // completetime �迭
int* wait_time; // waiting time �迭
int* turnaround_time; //turnaround time �迭

bool arrivecomp(const process& a, const process& b);
void Input();
void FCFS();
void SJF();
void Priority();
void RR();
void Print(double avr_wait, double avr_turnaround);

int main(void) {
	Input();
	FCFS();
	SJF();
	Priority();
	RR();

	// �޸� ����
	delete[]list;
	delete[]wait_time;
	delete[]processing_time;
	delete[]complete_time;
	delete[]turnaround_time;
	delete[]burst_time;

	system("pause");
	return 0;
}

// ���� �����ϴ� ������� ���� -> �����ð� ������ ���� ���μ��� ��ȣ ���� ����
bool arrivecomp(const process& a, const process& b) {
	return (a.arrive_t != b.arrive_t) ? (a.arrive_t < b.arrive_t) : (a.number < b.number);
}

void Input() {

	// ���μ��� ���� �Է�
	cout << "���μ��� ���� �Է�" << endl;
	while (1) {
		cin >> process_num;
		// ����ó��1 - ���μ��� ������ 1�̻�
		if (process_num > 0)
			break;
		cout << "����! ���μ��� ������ 1�̻����� �ٽ��Է��ϼ���." << endl;
	}

	// ���μ��� ������ŭ ����Ʈ �����Ҵ�
	list = new process[process_num];
	processing_time = new vector<processing>[process_num];
	burst_time = new int[process_num];
	complete_time = new int[process_num];
	wait_time = new int[process_num];
	turnaround_time = new int[process_num];

	// �� ���μ��� �� ���� �Է�
	process tmp;
	bool flag = false;
	while (1) {
		for (int i = 0; i < process_num; i++) {

			cout << endl << i + 1 << "�� ���μ��� ���� �Է�" << endl;
			cout << "Arrival Time / Burst Time / Priority " << endl;
			tmp.number = i + 1;

			// �Է� �ޱ�
			while (1) {
				cin >> tmp.arrive_t >> tmp.burst_t >> tmp.priority;
				// ����ó��2 - �����ΰ� �Է�
				if (tmp.arrive_t > -1 && tmp.burst_t > -1 && tmp.priority > -1)
					break;
				cout << "����! ������ ���� �Է��Ͽ����ϴ�. �ٽ��Է��ϼ���." << endl;
			}

			// burst time ����
			burst_time[i] = tmp.burst_t;

			// ����ó�� 4-(1) - �ϳ��� arrive�� 0�� ���μ����� �־���� -> ������ flag = true;
			if (tmp.arrive_t == 0)
				flag = true;

			// ����Ʈ�� ���μ��� ����
			list[i] = tmp;
		}
		// ����ó��4-(2) - arrive�� 0�� ���μ����� �ϳ��� ������ �ٽ� �Է�
		if (flag)
			break;
		cout << "����! ���� ���� �����ϴ� process�� arrive time�� 0���� ���缭 �Է��ϼ���!" << endl;
	}

	// Quantum�� �Է�
	while (1) {
		cout << endl << "Quantum �Է�:";
		cin >> quantum;
		// ����ó��3 quantum ���� 1�̻�
		if (quantum > 0)
			break;
		cout << "����! Quantum ���� 0���� Ŀ���մϴ�." << endl;
	}
}

void FCFS() {
	int complete = 0, wait = 0, turnaround = 0, start = 0;
	double avr_wait = 0.0, avr_turnaround = 0.0;
	processing tmp;

	// ����Ʈ �����ð� ������� ����(FCFS)
	sort(list, list + process_num, arrivecomp);

	// FCFS�� �Ǿ� process���� ó��
	for (int i = 0; i < process_num; i++) {

		// ����ó�� - ���μ��� ���� ���̿� ������ ����
		if (start < list[i].arrive_t) {
			cout << "FCFS �����ٸ� �߰��� ������ ������ϴ�. - �߸��� �Է��Դϴ�." << endl;
			system("pause");
			exit(1);
		}

		// ���� ó������ ���μ��� ����
		int number = list[i].number;
		int burst = list[i].burst_t;
		int arrive = list[i].arrive_t;

		// complete time (���� ���μ������� burst + ���� ���μ����� burst)
		complete += burst;
		complete_time[number - 1] = complete;

		// processing time
		tmp.start = start;
		tmp.end = complete;
		tmp.burst = tmp.end - tmp.start;
		processing_time[number - 1].push_back(tmp);

		// wait time (���� ���μ��� ���۽ð� - ���� ���μ��� �����ð�)
		wait = start - arrive;
		start = complete; // ���� ���μ����� start �ð� = ���� ���μ����� complete �ð�
		wait_time[number - 1] = wait;
		avr_wait += (double)wait;

		// turnaround time (���� ���μ��� ����ð� - ���� ���μ��� �����ð�) 
		turnaround = complete - arrive;
		turnaround_time[number - 1] = turnaround;
		avr_turnaround += (double)turnaround;

	}

	// Average waiting time & Average turnaround time
	avr_turnaround /= (double)process_num;
	avr_wait /= (double)process_num;

	// ��� ���
	cout << endl << "------------------------------------------------------FCFS------------------------------------------------------";
	Print(avr_wait, avr_turnaround);
}

void SJF() {
	process present, tmp;
	int total = 0, complete = 0, turnaround = 0, wait = 0, i = 0;
	double avr_wait = 0.0, avr_turnaround = 0.0;

	// processing_time ���� �ʱ�ȭ
	for (int i = 0; i < process_num; i++)
		processing_time[i].clear();

	// �غ�Ϸ� ť ����
	priority_queue<process, vector<process>, cmpburst> readyQ;

	// ����Ʈ �����ð� ������� ����
	sort(list, list + process_num, arrivecomp);

	// Slist - ����������� ���μ����� ���Ե� ť
	// ���μ����� �����ð��� �Ǹ� Slistť���� �غ�Ϸ� ť�� ���μ��� �̵�
	queue<process> Slist;
	for (int j = 0; j < process_num; j++) {
		Slist.push(list[j]);
		total += list[j].burst_t;
	}

	// �ʱ� ���μ��� ����
	present = Slist.front();
	Slist.pop();
	processing pt;
	pt.start = 0;

	// �����ٸ� ����
	while (i <= total) {

		// ������ ������ �κ�
		// ���� �ð�(i)���� Slist�� �����ð��� i�� process�� �ִٸ� ���� process�� burst time�� ��
		while (1) {
			if (Slist.size() != 0 && Slist.front().arrive_t == i) {
				tmp = Slist.front();
				Slist.pop();
				// ���� ó������ process ���� burst time�� ª���� ��ü (������ �Ͼ)
				if (tmp.burst_t < present.burst_t) {

					// ���� ���μ��� ��ü�Ǹ� processing time ���
					pt.end = i;
					pt.burst = i - pt.start;
					if (pt.burst != 0)
						processing_time[present.number - 1].push_back(pt);
					pt.start = i; // ���ο� ���۰� ����

					// ���ο� ���μ��� ������
					readyQ.push(present);
					present = tmp;
				}
				// �ƴϸ� readyQ�� push
				else readyQ.push(tmp);
			}
			else break;
		}

		// ���� ���μ��� burst time�� 0 -> ó�� �Ϸ�!
		if (present.burst_t == 0) {

			int number = present.number;
			int burst = present.burst_t;
			int arrive = present.arrive_t;

			// ���� ���μ����� complete, wait, turnaround ��
			complete = i;
			wait = complete - burst_time[number - 1] - arrive;
			turnaround = complete - arrive;
			complete_time[number - 1] = complete;
			turnaround_time[number - 1] = turnaround;
			wait_time[number - 1] = wait;

			// ��հ� ����� ���� ������ ���
			avr_wait += (double)wait;
			avr_turnaround += (double)turnaround;

			// ���� ���μ��� ������ processing time ���
			pt.end = i;
			pt.burst = i - pt.start;
			if (pt.burst != 0)
				processing_time[number - 1].push_back(pt);
			pt.start = i; // ���ο� ���۰� ����

			// readyQ�� �ִ� ���μ��� �� ���� ª�� burst time �� ���� ���μ����� ��ü
			if (readyQ.size() != 0) {
				present = readyQ.top();
				readyQ.pop();
			}
			// ����ó�� - ���� �������� ���μ����� �������� �غ�Ϸ� ť�� ���μ����� ����
			else if (i != total && readyQ.size() == 0) {
				cout << "SJF �����ٸ� �߰��� ������ ������ϴ�. - �߸��� �Է��Դϴ�.";
				system("pause");
				exit(1);
			}
			// ������ ���μ��� �� -> readyQ�� ���̻� ���ٸ� ����
			else break;
		}

		// �ð��� 1�� ����
		present.burst_t -= 1; // ���� ó������ ���μ����� burst 1 ����
		i = i + 1; // �ð��� 1 ����
	}

	// Average waiting time & Average turnaround time
	avr_turnaround /= (double)process_num;
	avr_wait /= (double)process_num;

	// ��� ���
	cout << endl << "------------------------------------------------------SJF------------------------------------------------------";
	Print(avr_wait, avr_turnaround);
}

void Priority() {
	process present, tmp;
	int total = 0, complete = 0, turnaround = 0, wait = 0, i = 0;
	double avr_wait = 0.0, avr_turnaround = 0.0;

	// processing_time ���� �ʱ�ȭ
	for (int i = 0; i < process_num; i++)
		processing_time[i].clear();

	// ť ����
	priority_queue<process, vector<process>, cmppriority> readyQ;

	// ����Ʈ �����ð� ������� ����
	sort(list, list + process_num, arrivecomp);

	// ����Ʈ ����
	queue<process> Plist;
	for (int j = 0; j < process_num; j++) {
		Plist.push(list[j]);
		total += list[j].burst_t;
	}

	// �ʱ� ���μ��� ����
	present = Plist.front();
	Plist.pop();
	processing pt;
	pt.start = 0;

	while (i <= total) {
		// ���� �ð�(i)���� arrive time�� i�� process�� ������ ���� ���μ����� priority�� ��
		while (1) {
			if (Plist.size() != 0 && Plist.front().arrive_t == i) {
				tmp = Plist.front();
				Plist.pop();

				// ���� ó������ process ���� Priority�� ������ ��ü
				if (tmp.priority < present.priority) {
					// ���� ���μ��� ��ü�Ǹ� processing time ���
					pt.end = i;
					pt.burst = i - pt.start;
					if (pt.burst != 0)
						processing_time[present.number - 1].push_back(pt);
					pt.start = i; // ���ο� ���۰� ����
					// ���μ��� ��ü
					readyQ.push(present);
					present = tmp;
				}
				// �ƴϸ� readyQ�� push
				else
					readyQ.push(tmp);
			}
			else
				break;
		}

		// ���� ���μ��� burst time�� 0 -> ó�� �Ϸ�!
		if (present.burst_t == 0) {

			int number = present.number;
			int burst = present.burst_t;
			int arrive = present.arrive_t;

			// ���� ���μ����� complete, wait, turnaround ��
			complete = i;
			wait = complete - burst_time[number - 1] - arrive;
			turnaround = complete - arrive;
			complete_time[number - 1] = complete;
			turnaround_time[number - 1] = turnaround;
			wait_time[number - 1] = wait;

			// ��հ� ����� ���� ������ ���
			avr_wait += (double)wait;
			avr_turnaround += (double)turnaround;

			// ���� ���μ��� ������ processing time ���
			pt.end = i;
			pt.burst = i - pt.start;
			if (pt.burst != 0)
				processing_time[number - 1].push_back(pt);
			pt.start = i; // ���ο� ���۰� ����

			// readyQ�� �ִ� ���μ��� �� ���� ª�� burst time �� ���� ���μ����� ��ü
			if (readyQ.size() != 0) {
				present = readyQ.top();
				readyQ.pop();
			}
			// ����ó�� - ���� �������� ���μ����� �������� �غ�Ϸ� ť�� ���μ����� ����
			else if (i != total && readyQ.size() == 0) {
				cout << "Priority �����ٸ� �߰��� ������ ������ϴ�. - �߸��� �Է��Դϴ�.";
				system("pause");
				exit(1);
			}
			// readyQ�� ���̻� ���ٸ� ����
			else {
				break;
			}
		}

		// �ð��� 1�� ����
		present.burst_t -= 1;
		i = i + 1;
	}

	// Average waiting time & Average turnaround time
	avr_turnaround /= (double)process_num;
	avr_wait /= (double)process_num;

	// ��� ���
	cout << endl << "----------------------------------------------------Priority---------------------------------------------------";
	Print(avr_wait, avr_turnaround);
}

void RR() {
	process present; //���� ó������ ť
	processing pt;
	int total = 0, complete = 0, turnaround = 0, wait = 0, i = 0;
	double avr_wait = 0.0, avr_turnaround = 0.0;

	// processing_time ���� �ʱ�ȭ
	for (int i = 0; i < process_num; i++)
		processing_time[i].clear();

	// ���� ť ����
	queue<process> readyQ;

	// ����Ʈ �����ð� ������� ����
	sort(list, list + process_num, arrivecomp);

	// process���� Rlist�� ����
	queue<process> Rlist;
	for (int j = 0; j < process_num; j++) {
		Rlist.push(list[j]);
		total += list[j].burst_t;
	}

	// ���μ��� ó��
	while (i < total) {

		// ���� �ð�(i)���� arrive time�� i�� process�� ������ ��� readyQ�� �־��ش�
		while (1) {
			if (Rlist.size() != 0 && Rlist.front().arrive_t == i) {
				readyQ.push(Rlist.front());
				Rlist.pop();
			}
			else
				break;
		}

		// ����ó�� - �غ�Ϸ� ť�� �ƹ��͵� ���� 
		if (i != total && readyQ.size() == 0) {
			cout << "RR�˰��� �߰��� ������ ������ϴ�. - �߸��� �Է��Դϴ�.";
			system("pause");
			exit(1);
		}

		// ���� ó���� process�� readyQ���� �ϳ� ������
		present = readyQ.front();
		readyQ.pop();
		int number = present.number;
		int burst = present.burst_t;
		int arrive = present.arrive_t;
		pt.start = i;

		// burst <= quantum �̸� ���μ��� ó�� ����
		if (burst <= quantum) {

			// ���� burst ��ŭ �����ϴ� ���� ���� ���μ����� readyQ�� push
			while (1) {
				if (Rlist.size() != 0 && Rlist.front().arrive_t < i + burst) {
					readyQ.push(Rlist.front());
					Rlist.pop();
				}
				else
					break;
			}

			// ���� ���μ��� ������ processing time ���
			pt.end = pt.start + burst;
			pt.burst = burst;
			if (pt.burst != 0)
				processing_time[number - 1].push_back(pt);
			pt.start = pt.end; // ���ο� ���۰� ����

			// ���� process�� complete, turnaround, wait �� 
			complete = i + burst; // ����ð� + ���� burst
			turnaround = complete - arrive;
			wait = complete - arrive - burst_time[number - 1];
			complete_time[number - 1] = complete;
			turnaround_time[number - 1] = turnaround;
			wait_time[number - 1] = wait;

			// ��հ� ����� ���� �� ����
			avr_wait += (double)wait;
			avr_turnaround += (double)turnaround;

			// burst��ŭ ���� �ð����� �̵�
			i = i + burst;
		}

		// quantum���� �� ���� �������� quantum��ŭ ���μ��� ó��
		else {
			// ���� process�� quantum ��ŭ �����ϴ� ���� ���� ���μ����� readyQ�� push
			while (1) {
				if (Rlist.size() != 0 && Rlist.front().arrive_t < i + quantum) {
					readyQ.push(Rlist.front());
					Rlist.pop();
				}
				else
					break;
			}

			// ���� ���μ��� ��ü�� processing time ���
			pt.end = pt.start + quantum;
			pt.burst = quantum;
			processing_time[number - 1].push_back(pt);
			pt.start = pt.end; // ���ο� ���۰� ����

			// ���� ���μ��� ó�� �� �ٽ� readyQ�� ����
			present.burst_t -= quantum;
			readyQ.push(present);
			i = i + quantum;
		}
	}

	//Average Turnaround & Average Waiting
	avr_turnaround /= (double)process_num;
	avr_wait /= (double)process_num;

	// ��� ���
	cout << endl << "------------------------------------------------------RR------------------------------------------------------";
	Print(avr_wait, avr_turnaround);
}

void Print(double avr_wait, double avr_turnaround) {
	cout << endl << "<Processing Time>          " << endl;
	for (int i = 0; i < process_num; i++) {
		cout << "P" << i + 1 << " : ";
		for (int j = 0; j < processing_time[i].size(); j++)
			cout << "(" << processing_time[i][j].start << " ~ " << processing_time[i][j].end << " " << processing_time[i][j].burst << "����) ";
		cout << endl;
	}
	cout << "<Wait Time>                ";
	for (int i = 0; i < process_num; i++)
		cout << "P" << i + 1 << ": " << wait_time[i] << "  ";
	cout << endl << "<Complete Time>            ";
	for (int i = 0; i < process_num; i++)
		cout << "P" << i + 1 << ": " << complete_time[i] << "  ";
	cout << endl << "<Average Waiting Time>     " << avr_wait << endl;
	cout << "<Average Turnaround Time>  " << avr_turnaround << endl;
}