#include <iostream>
using namespace std;

class queue{
	int size;
	int *arr;
	int r, f;
	public:
		queue(int s){
			this->size = s;
			arr = new int[size];
			r = f = -1;
		}
		void push(int i){
				if(r == -1){
					r++;
					r = r% size;
					f++;
					f = f % size;
					arr[r] = i;
				}	
				else{
					if((r+1)<size){
						r++;
						r = r % size;
						arr[r] = i;
					}
					else{
						cout<<"Queue full";
					}
				}
		}
		int pop(){
			int temp;
			if(r == f){
				temp = arr[r];
				r = -1;
				f = -1;
			}
			else{
				temp = arr[f];
				f++;
				f = f % size;
			}
			return temp;
		}
		bool isempty(){
			if(r == -1)
				return true;
			
			return false;
		}
};

class Task{
	int id;
	int AT;
	int BT;
	int priority;

public:
	Task(){
		id = AT = BT = priority = 0;
	}
	Task(Task &t){
		id = t.id;
		AT = t.AT;
		BT = t.BT;
		priority = t.priority;
		
	}
	friend class Scheduling;
	friend class pqueue;
	friend class pqueueAT;
};
//priority queue using BT
class pqueue{
	Task *t;
	int size;
	int f;
	int r;
public:
	pqueue(int size){
		this->size = size;
		t = new Task[size];
		f = -1;
		r = -1;
	}
	void push(Task task){
		if(r == -1){
			f = r = 0;
			t[r] = task;
			return;
		}
		
		r =(r + 1)%size;
		
		if(r == f){
			cout<<"Queue overloaded\n";
			return;
		}
		int temp = r;
		while(t[(temp-1)%size].BT > task.BT){
			if(temp == f){
				break;
			}
			t[temp] = t[(temp-1)%size];
			temp = (temp - 1)%size;
		}
		t[temp] = task;
		
	}
	Task top(){
		Task temp;
		if(r == -1){
			cout<<"Queue empty\n";
			return temp;
		}
		else{
			return t[r];
		}
	}
	
	Task pop(){
		Task temp;
		if(r == -1){
			cout<<"Queue empty\n";
			return temp;
		}
		if(r == f){
			temp = t[r];
			f = r = -1;
			return temp;
		}
		temp = t[f];
		f = (f+1)%size;
		return temp;
	}
	bool isempty(){
		if(r == -1){
			return true;
		}
		return false;
	}
};
//priority queue AT
class pqueueAT{
	Task *t;
	int size;
	int f;
	int r;
public:
	pqueueAT(int size){
		this->size = size;
		t = new Task[size];
		f = -1;
		r = -1;
	}
	void push(Task task){
		if(r == -1){
			f = r = 0;
			t[r] = task;
			return;
		}
		
		r =(r + 1)%size;
		
		if(r == f){
			cout<<"Queue overloaded\n";
			return;
		}
		int temp = r;
		while(t[(temp-1)%size].AT > task.AT){
			t[temp] = t[(temp-1)%size];
			temp = (temp - 1)%size;
		}
		t[temp] = task;
		
	}
	Task top(){
		Task temp;
		if(r == -1){
			cout<<"Queue empty\n";
			return temp;
		}
		else{
			return t[r];
		}
	}
	
	Task pop(){
		Task temp;
		if(r == -1){
			cout<<"Queue empty\n";
			return temp;
		}
		if(r == f){
			temp = t[r];
			f = r = -1;
			return temp;
		}
		temp = t[f];
		f = (f+1)%size;
		return temp;
	}
	bool isempty(){
		if(r == -1){
			return true;
		}
		return false;
	}
};



class Scheduling{
	Task *tasks;
	int size;
	void getTasks(int n);
	void FCFS();
	void SJF();
	void RoundRobin();
	void Priority();
public:
	Scheduling(){
		cout<<"Enter number of Tasks: ";
		cin>>this->size;
		this->tasks = new Task[size];
		getTasks(size);
		}
	void algos(){
		int c;
		cout<<"Enter 1 for FCFS\n";
		cin>>c;
		if(c == 1)
			FCFS();
		else if(c == 2)
			SJF();
		else if(c == 3)
			RoundRobin();
		else if(c == 4)
			Priority();
		else if(c == 5){
			FCFS();
			SJF();
			RoundRobin();
			Priority();

		}
	}
};

void Scheduling::getTasks(int n){
		for(int i = 0; i<n; i++){
			tasks[i].id = i;
			cout<<"Arrival Time for task "<<i<<" ";
			cin>>tasks[i].AT;
			cout<<"Burst Time for task "<<i<<" ";
			cin>>tasks[i].BT;
			cout<<"Priority of task "<<i<<" ";
			cin>>tasks[i].priority;
	}
}

void Scheduling::FCFS(){
	queue readyq(size);
	int CT[size];
	for(int i = 0; i<size; i++){
		readyq.push(tasks[i].id);
	}
	int time = 0;
	
	while(!readyq.isempty()){
		int index = readyq.pop();
		int bt = tasks[index].BT;
		if(time < tasks[index].AT ){
			time = tasks[index].AT;
		}
		time += bt;
		CT[index] = time;
	}
	cout<<"--------------------------------------------------------\n";
	cout<<"FCFS\n";
	float tatsum = 0;
	float wtsum = 0;
	cout<<"ID	AT	BT	CT	TpopAT	WT\n";
	for(int i = 0; i<size; i++){
		Task t = tasks[i];
		tatsum += CT[i] - t.AT;
		wtsum += CT[i]-t.AT - t.BT;
		cout<<t.id<<"	"<<t.AT<<"	"<<t.BT<<"	"<<CT[i]<<"	"<<CT[i] - t.AT<<"	"<<CT[i]-t.AT - t.BT<<endl;
		}
	cout<<"Average TAT : "<<tatsum/size<<endl;
	cout<<"Average WT: "<<wtsum/size<<endl;
	cout<<"---------------------------------------------------------\n";
}

void Scheduling::SJF(){
	pqueue readyq(size);
	int currenttime = 0;
	int CT[size];
	Task sjt[size];
	for(int i = 0; i<size; i++){
		sjt[i] = tasks[i];
		CT[i] = 0;
	}
	int i = 0;
	currenttime = 0;
	Task temp;
	int count = 0;
	while(!readyq.isempty()|| i < size ){
		if(i<= size){
			while(currenttime >= sjt[i].AT){
				readyq.push(sjt[i]);
				i++;
				if(i>=size){
					break;
				}
			}
		}
		if(!readyq.isempty()){
			temp = readyq.pop();
			sjt[temp.id].BT = temp.BT - 1;
			if(sjt[temp.id].BT != 0){
				readyq.push(sjt[temp.id]);
			}
			else{
				CT[temp.id] = currenttime + 1;
			}
		}
		currenttime++;
	}
	cout<<"--------------------------------------------------------\n";
		cout<<"SJF\n";
		float tatsum = 0;
		float wtsum = 0;
		cout<<"ID	AT	BT	CT	TAT	WT\n";
		for(int i = 0; i<size; i++){
			Task t = tasks[i];
			tatsum += CT[i] - t.AT;
			wtsum += CT[i]-t.AT - t.BT;
			cout<<t.id<<"	"<<t.AT<<"	"<<t.BT<<"	"<<CT[i]<<"	"<<CT[i] - t.AT<<"	"<<CT[i]-t.AT - t.BT<<endl;
			}
		cout<<"Average TAT : "<<tatsum/size<<endl;
		cout<<"Average WT: "<<wtsum/size<<endl;
		cout<<"---------------------------------------------------------\n";
}

void Scheduling::RoundRobin(){
	queue readyq(size);
	Task rr[size];
	int CT[size];
	for(int i = 0; i<size; i++){
		rr[i] = tasks[i];
		CT[i] = 0;
	}
	int cputime = 0;
	Task* t;

	cout<<"Enter TQ:";
	int tq;
	bool flag = false;
	cin>>tq;
	int i = 0;
	while(!readyq.isempty() || i<size){
		if(i<size){
			while(rr[i].AT <= cputime){
				readyq.push(rr[i].id);
				i++;
				if(i>= size){
					break;
				}
			}
		}
		if(flag){
		readyq.push(t->id);
		flag = false;
		}
		if(readyq.isempty()){
			cputime += 1;
			continue;
		}
		else{
		t = &rr[readyq.pop()];
		if(t->BT <= tq){
			cputime += t->BT;
			CT[t->id] = cputime;

		}
		else{
		t->BT = t->BT - tq;
		flag = true;
		cputime+=tq;
		}
		}

	}
	if(!CT[t->id]){
		CT[t->id] = cputime + t->BT;
	}
	cout<<"--------------------------------------------------------\n";
	cout<<"RR\n";
	float tatsum = 0;
	float wtsum = 0;
	cout<<"ID	AT	BT	CT	TAT	WT\n";
	for(int i = 0; i<size; i++){
		Task t = tasks[i];
		tatsum += CT[i] - t.AT;
		wtsum += CT[i]-t.AT - t.BT;
		cout<<t.id<<"	"<<t.AT<<"	"<<t.BT<<"	"<<CT[i]<<"	"<<CT[i] - t.AT<<"	"<<CT[i]-t.AT - t.BT<<endl;
		}
	cout<<"Average TAT : "<<tatsum/size<<endl;
	cout<<"Average WT: "<<wtsum/size<<endl;
	cout<<"---------------------------------------------------------\n";
}

void Scheduling::Priority(){
	int CT[size];
	pqueueAT readyq(size);
	Task pt[size];
	for(int i = 0; i<size; i++){
		pt[i] = tasks[i];
		CT[i] = 0;
	}
	int currenttime = 0;
	int i = 0;
	Task temp;
	bool flag = true;
	while(!readyq.isempty()|| i < size ){
		flag = true;
		if(i<=size){
		while(currenttime >= pt[i].AT){
			readyq.push(pt[i]);
			i++;
			if(i>=size){
				break;
			}
		}}
		if(!readyq.isempty()){
			temp = readyq.pop();
			currenttime = currenttime + temp.BT;
			CT[temp.id] = currenttime;
			flag = false;
		}
		if(flag)
			currenttime++;
				
	}
	cout<<"--------------------------------------------------------\n";
		cout<<"Priority\n";
		float tatsum = 0;
		float wtsum = 0;
		cout<<"ID	AT	BT	CT	TAT	WT\n";
		for(int i = 0; i<size; i++){
			Task t = tasks[i];
			tatsum += CT[i] - t.AT;
			wtsum += CT[i]-t.AT - t.BT;
			cout<<t.id<<"	"<<t.AT<<"	"<<t.BT<<"	"<<CT[i]<<"	"<<CT[i] - t.AT<<"	"<<CT[i]-t.AT - t.BT<<endl;
			}
		cout<<"Average TAT : "<<tatsum/size<<endl;
		cout<<"Average WT: "<<wtsum/size<<endl;
		cout<<"---------------------------------------------------------\n";

}

int main(){
	Scheduling s;
	s.algos();
}
//6 0 2 1 4 3 2 8 4 1 10 5 3 11 2 1 12 1 2