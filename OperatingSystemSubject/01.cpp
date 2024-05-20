#include <iostream>
#include <algorithm>
#include <queue>
#include <stdlib.h>
using namespace std;

// 2024-05-21-01:19 Commit

// 프로세스의 정보를 담은 구조체
typedef struct process {
	int number = 0; // 프로세스 번호
	int arrive_t = 0; // 도착시간
	int burst_t = 0; // 실행시간
	int priority = 0; // 우선순위
}process;

// processing time 출력을 위한 구조체
typedef struct processing {
	int start;
	int end;
	int burst;
}processing;

// 실행시간 정렬
struct cmpburst {
	bool operator()(process a, process b) {
		return a.burst_t > b.burst_t;
	}
};

// 우선순위 정렬
struct cmppriority {
	bool operator()(process a, process b) {
		return a.priority > b.priority;
	}
};

int process_num; // 프로세스 개수
int quantum = 1; // quantum
process* list; // 각 프로세스를 저장한 리스트
vector<processing>* processing_time; // processingtime 배열
int* burst_time; // bursttime 배열
int* complete_time; // completetime 배열
int* wait_time; // waiting time 배열
int* turnaround_time; //turnaround time 배열

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

	// 메모리 해제
	delete[]list;
	delete[]wait_time;
	delete[]processing_time;
	delete[]complete_time;
	delete[]turnaround_time;
	delete[]burst_time;

	system("pause");
	return 0;
}

// 먼저 도착하는 순서대로 정렬 -> 도착시간 같으면 낮은 프로세스 번호 먼저 실행
bool arrivecomp(const process& a, const process& b) {
	return (a.arrive_t != b.arrive_t) ? (a.arrive_t < b.arrive_t) : (a.number < b.number);
}

void Input() {

	// 프로세스 개수 입력
	cout << "프로세스 개수 입력" << endl;
	while (1) {
		cin >> process_num;
		// 예외처리1 - 프로세스 개수는 1이상
		if (process_num > 0)
			break;
		cout << "오류! 프로세스 개수를 1이상으로 다시입력하세요." << endl;
	}

	// 프로세스 개수만큼 리스트 동적할당
	list = new process[process_num];
	processing_time = new vector<processing>[process_num];
	burst_time = new int[process_num];
	complete_time = new int[process_num];
	wait_time = new int[process_num];
	turnaround_time = new int[process_num];

	// 각 프로세스 별 정보 입력
	process tmp;
	bool flag = false;
	while (1) {
		for (int i = 0; i < process_num; i++) {

			cout << endl << i + 1 << "번 프로세스 정보 입력" << endl;
			cout << "Arrival Time / Burst Time / Priority " << endl;
			tmp.number = i + 1;

			// 입력 받기
			while (1) {
				cin >> tmp.arrive_t >> tmp.burst_t >> tmp.priority;
				// 예외처리2 - 음수인값 입력
				if (tmp.arrive_t > -1 && tmp.burst_t > -1 && tmp.priority > -1)
					break;
				cout << "오류! 음수인 값을 입력하였습니다. 다시입력하세요." << endl;
			}

			// burst time 저장
			burst_time[i] = tmp.burst_t;

			// 예외처리 4-(1) - 하나라도 arrive가 0인 프로세스가 있어야함 -> 있으면 flag = true;
			if (tmp.arrive_t == 0)
				flag = true;

			// 리스트에 프로세스 저장
			list[i] = tmp;
		}
		// 예외처리4-(2) - arrive가 0인 프로세스가 하나도 없으면 다시 입력
		if (flag)
			break;
		cout << "오류! 가장 빨리 도착하는 process의 arrive time을 0으로 맞춰서 입력하세요!" << endl;
	}

	// Quantum값 입력
	while (1) {
		cout << endl << "Quantum 입력:";
		cin >> quantum;
		// 예외처리3 quantum 값은 1이상
		if (quantum > 0)
			break;
		cout << "오류! Quantum 값이 0보다 커야합니다." << endl;
	}
}

void FCFS() {
	int complete = 0, wait = 0, turnaround = 0, start = 0;
	double avr_wait = 0.0, avr_turnaround = 0.0;
	processing tmp;

	// 리스트 도착시간 순서대로 정렬(FCFS)
	sort(list, list + process_num, arrivecomp);

	// FCFS로 맨앞 process부터 처리
	for (int i = 0; i < process_num; i++) {

		// 예외처리 - 프로세스 실행 사이에 공백이 있음
		if (start < list[i].arrive_t) {
			cout << "FCFS 스케줄링 중간에 공백이 생겼습니다. - 잘못된 입력입니다." << endl;
			system("pause");
			exit(1);
		}

		// 현재 처리중인 프로세스 정보
		int number = list[i].number;
		int burst = list[i].burst_t;
		int arrive = list[i].arrive_t;

		// complete time (이전 프로세스들의 burst + 현재 프로세스의 burst)
		complete += burst;
		complete_time[number - 1] = complete;

		// processing time
		tmp.start = start;
		tmp.end = complete;
		tmp.burst = tmp.end - tmp.start;
		processing_time[number - 1].push_back(tmp);

		// wait time (현재 프로세스 시작시간 - 현재 프로세스 도착시간)
		wait = start - arrive;
		start = complete; // 다음 프로세스의 start 시간 = 현재 프로세스의 complete 시간
		wait_time[number - 1] = wait;
		avr_wait += (double)wait;

		// turnaround time (현재 프로세스 종료시간 - 현재 프로세스 도착시간) 
		turnaround = complete - arrive;
		turnaround_time[number - 1] = turnaround;
		avr_turnaround += (double)turnaround;

	}

	// Average waiting time & Average turnaround time
	avr_turnaround /= (double)process_num;
	avr_wait /= (double)process_num;

	// 결과 출력
	cout << endl << "------------------------------------------------------FCFS------------------------------------------------------";
	Print(avr_wait, avr_turnaround);
}

void SJF() {
	process present, tmp;
	int total = 0, complete = 0, turnaround = 0, wait = 0, i = 0;
	double avr_wait = 0.0, avr_turnaround = 0.0;

	// processing_time 벡터 초기화
	for (int i = 0; i < process_num; i++)
		processing_time[i].clear();

	// 준비완료 큐 생성
	priority_queue<process, vector<process>, cmpburst> readyQ;

	// 리스트 도착시간 순서대로 정렬
	sort(list, list + process_num, arrivecomp);

	// Slist - 도착순서대로 프로세스가 삽입된 큐
	// 프로세스가 도착시간이 되면 Slist큐에서 준비완료 큐로 프로세스 이동
	queue<process> Slist;
	for (int j = 0; j < process_num; j++) {
		Slist.push(list[j]);
		total += list[j].burst_t;
	}

	// 초기 프로세스 설정
	present = Slist.front();
	Slist.pop();
	processing pt;
	pt.start = 0;

	// 스케줄링 진행
	while (i <= total) {

		// 선점을 구현한 부분
		// 현재 시간(i)에서 Slist에 도착시간이 i인 process가 있다면 현재 process와 burst time을 비교
		while (1) {
			if (Slist.size() != 0 && Slist.front().arrive_t == i) {
				tmp = Slist.front();
				Slist.pop();
				// 현재 처리중인 process 보다 burst time이 짧으면 교체 (선점이 일어남)
				if (tmp.burst_t < present.burst_t) {

					// 현재 프로세스 교체되면 processing time 계산
					pt.end = i;
					pt.burst = i - pt.start;
					if (pt.burst != 0)
						processing_time[present.number - 1].push_back(pt);
					pt.start = i; // 새로운 시작값 저장

					// 새로운 프로세스 꺼내옴
					readyQ.push(present);
					present = tmp;
				}
				// 아니면 readyQ로 push
				else readyQ.push(tmp);
			}
			else break;
		}

		// 현재 프로세스 burst time이 0 -> 처리 완료!
		if (present.burst_t == 0) {

			int number = present.number;
			int burst = present.burst_t;
			int arrive = present.arrive_t;

			// 현재 프로세스의 complete, wait, turnaround 값
			complete = i;
			wait = complete - burst_time[number - 1] - arrive;
			turnaround = complete - arrive;
			complete_time[number - 1] = complete;
			turnaround_time[number - 1] = turnaround;
			wait_time[number - 1] = wait;

			// 평균값 계산을 위한 누적합 계산
			avr_wait += (double)wait;
			avr_turnaround += (double)turnaround;

			// 현재 프로세스 끝나면 processing time 계산
			pt.end = i;
			pt.burst = i - pt.start;
			if (pt.burst != 0)
				processing_time[number - 1].push_back(pt);
			pt.start = i; // 새로운 시작값 저장

			// readyQ에 있는 프로세스 중 가장 짧은 burst time 을 가진 프로세스로 교체
			if (readyQ.size() != 0) {
				present = readyQ.top();
				readyQ.pop();
			}
			// 예외처리 - 현재 진행중인 프로세스가 끝났지만 준비완료 큐에 프로세스가 없다
			else if (i != total && readyQ.size() == 0) {
				cout << "SJF 스케줄링 중간에 공백이 생겼습니다. - 잘못된 입력입니다.";
				system("pause");
				exit(1);
			}
			// 마지막 프로세스 끝 -> readyQ에 더이상 없다면 종료
			else break;
		}

		// 시간이 1초 지남
		present.burst_t -= 1; // 현재 처리중인 프로세스의 burst 1 감소
		i = i + 1; // 시간은 1 지남
	}

	// Average waiting time & Average turnaround time
	avr_turnaround /= (double)process_num;
	avr_wait /= (double)process_num;

	// 결과 출력
	cout << endl << "------------------------------------------------------SJF------------------------------------------------------";
	Print(avr_wait, avr_turnaround);
}

void Priority() {
	process present, tmp;
	int total = 0, complete = 0, turnaround = 0, wait = 0, i = 0;
	double avr_wait = 0.0, avr_turnaround = 0.0;

	// processing_time 벡터 초기화
	for (int i = 0; i < process_num; i++)
		processing_time[i].clear();

	// 큐 생성
	priority_queue<process, vector<process>, cmppriority> readyQ;

	// 리스트 도착시간 순서대로 정렬
	sort(list, list + process_num, arrivecomp);

	// 리스트 복사
	queue<process> Plist;
	for (int j = 0; j < process_num; j++) {
		Plist.push(list[j]);
		total += list[j].burst_t;
	}

	// 초기 프로세스 설정
	present = Plist.front();
	Plist.pop();
	processing pt;
	pt.start = 0;

	while (i <= total) {
		// 현재 시간(i)에서 arrive time이 i인 process가 있을때 현재 프로세스위 priority를 비교
		while (1) {
			if (Plist.size() != 0 && Plist.front().arrive_t == i) {
				tmp = Plist.front();
				Plist.pop();

				// 현재 처리중인 process 보다 Priority이 높으면 교체
				if (tmp.priority < present.priority) {
					// 현재 프로세스 교체되면 processing time 계산
					pt.end = i;
					pt.burst = i - pt.start;
					if (pt.burst != 0)
						processing_time[present.number - 1].push_back(pt);
					pt.start = i; // 새로운 시작값 저장
					// 프로세스 교체
					readyQ.push(present);
					present = tmp;
				}
				// 아니면 readyQ로 push
				else
					readyQ.push(tmp);
			}
			else
				break;
		}

		// 현재 프로세스 burst time이 0 -> 처리 완료!
		if (present.burst_t == 0) {

			int number = present.number;
			int burst = present.burst_t;
			int arrive = present.arrive_t;

			// 현재 프로세스의 complete, wait, turnaround 값
			complete = i;
			wait = complete - burst_time[number - 1] - arrive;
			turnaround = complete - arrive;
			complete_time[number - 1] = complete;
			turnaround_time[number - 1] = turnaround;
			wait_time[number - 1] = wait;

			// 평균값 계산을 위한 누적값 계산
			avr_wait += (double)wait;
			avr_turnaround += (double)turnaround;

			// 현재 프로세스 끝나면 processing time 계산
			pt.end = i;
			pt.burst = i - pt.start;
			if (pt.burst != 0)
				processing_time[number - 1].push_back(pt);
			pt.start = i; // 새로운 시작값 저장

			// readyQ에 있는 프로세스 중 가장 짧은 burst time 을 가진 프로세스로 교체
			if (readyQ.size() != 0) {
				present = readyQ.top();
				readyQ.pop();
			}
			// 예외처리 - 현재 진행중인 프로세스가 끝났지만 준비완료 큐에 프로세스가 없다
			else if (i != total && readyQ.size() == 0) {
				cout << "Priority 스케줄링 중간에 공백이 생겼습니다. - 잘못된 입력입니다.";
				system("pause");
				exit(1);
			}
			// readyQ에 더이상 없다면 종료
			else {
				break;
			}
		}

		// 시간이 1초 지남
		present.burst_t -= 1;
		i = i + 1;
	}

	// Average waiting time & Average turnaround time
	avr_turnaround /= (double)process_num;
	avr_wait /= (double)process_num;

	// 결과 출력
	cout << endl << "----------------------------------------------------Priority---------------------------------------------------";
	Print(avr_wait, avr_turnaround);
}

void RR() {
	process present; //현재 처리중인 큐
	processing pt;
	int total = 0, complete = 0, turnaround = 0, wait = 0, i = 0;
	double avr_wait = 0.0, avr_turnaround = 0.0;

	// processing_time 벡터 초기화
	for (int i = 0; i < process_num; i++)
		processing_time[i].clear();

	// 레디 큐 생성
	queue<process> readyQ;

	// 리스트 도착시간 순서대로 정렬
	sort(list, list + process_num, arrivecomp);

	// process들을 Rlist로 복사
	queue<process> Rlist;
	for (int j = 0; j < process_num; j++) {
		Rlist.push(list[j]);
		total += list[j].burst_t;
	}

	// 프로세스 처리
	while (i < total) {

		// 현재 시간(i)에서 arrive time이 i인 process가 있으면 모두 readyQ에 넣어준다
		while (1) {
			if (Rlist.size() != 0 && Rlist.front().arrive_t == i) {
				readyQ.push(Rlist.front());
				Rlist.pop();
			}
			else
				break;
		}

		// 예외처리 - 준비완료 큐에 아무것도 없음 
		if (i != total && readyQ.size() == 0) {
			cout << "RR알고리즘 중간에 공백이 생겼습니다. - 잘못된 입력입니다.";
			system("pause");
			exit(1);
		}

		// 현재 처리할 process를 readyQ에서 하나 꺼낸다
		present = readyQ.front();
		readyQ.pop();
		int number = present.number;
		int burst = present.burst_t;
		int arrive = present.arrive_t;
		pt.start = i;

		// burst <= quantum 이면 프로세스 처리 종료
		if (burst <= quantum) {

			// 남은 burst 만큼 진행하는 동안 들어온 프로세스들 readyQ에 push
			while (1) {
				if (Rlist.size() != 0 && Rlist.front().arrive_t < i + burst) {
					readyQ.push(Rlist.front());
					Rlist.pop();
				}
				else
					break;
			}

			// 현재 프로세스 끝나면 processing time 계산
			pt.end = pt.start + burst;
			pt.burst = burst;
			if (pt.burst != 0)
				processing_time[number - 1].push_back(pt);
			pt.start = pt.end; // 새로운 시작값 저장

			// 현재 process의 complete, turnaround, wait 값 
			complete = i + burst; // 현재시간 + 남은 burst
			turnaround = complete - arrive;
			wait = complete - arrive - burst_time[number - 1];
			complete_time[number - 1] = complete;
			turnaround_time[number - 1] = turnaround;
			wait_time[number - 1] = wait;

			// 평균값 계산을 위해 값 누적
			avr_wait += (double)wait;
			avr_turnaround += (double)turnaround;

			// burst만큼 다음 시간으로 이동
			i = i + burst;
		}

		// quantum보다 더 많이 남았으면 quantum만큼 프로세스 처리
		else {
			// 현재 process를 quantum 만큼 진행하는 동안 들어온 프로세스들 readyQ에 push
			while (1) {
				if (Rlist.size() != 0 && Rlist.front().arrive_t < i + quantum) {
					readyQ.push(Rlist.front());
					Rlist.pop();
				}
				else
					break;
			}

			// 현재 프로세스 교체시 processing time 계산
			pt.end = pt.start + quantum;
			pt.burst = quantum;
			processing_time[number - 1].push_back(pt);
			pt.start = pt.end; // 새로운 시작값 저장

			// 현재 프로세스 처리 후 다시 readyQ로 삽입
			present.burst_t -= quantum;
			readyQ.push(present);
			i = i + quantum;
		}
	}

	//Average Turnaround & Average Waiting
	avr_turnaround /= (double)process_num;
	avr_wait /= (double)process_num;

	// 결과 출력
	cout << endl << "------------------------------------------------------RR------------------------------------------------------";
	Print(avr_wait, avr_turnaround);
}

void Print(double avr_wait, double avr_turnaround) {
	cout << endl << "<Processing Time>          " << endl;
	for (int i = 0; i < process_num; i++) {
		cout << "P" << i + 1 << " : ";
		for (int j = 0; j < processing_time[i].size(); j++)
			cout << "(" << processing_time[i][j].start << " ~ " << processing_time[i][j].end << " " << processing_time[i][j].burst << "실행) ";
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