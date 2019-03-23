#include <iostream>
#include "output.h"
#include <iomanip>

using namespace std;

void help() {
  cout << "Run with input file as an argument to sum the numbers in the input file." << endl;
  cout << "Flags:" << endl;
  cout << "   " << "-o, --output: write output to file" << endl;
  cout << "   " << "-t, --per_thread: display per-thread information" << endl;
  cout << "   " << "-v, --verbose: display all information about processes" << endl;
}

void printThread(int pid, int tid, int ptype) {
  cout << "    Thread " << tid << " ";
  cout << "in process " << pid;
  printProcessType(true, ptype);
}

void printProcessType(bool b, int ptype){
  if (b) {
    cout << " [";
  }
  switch (ptype){
    case 0:
    cout << "SYSTEM";
    break;
    case 1:
    cout << "INTERACTIVE";
    break;
    case 2:
    cout << "NORMAL";
    break;
    case 3:
    cout << "BATCH";
    break;
    default:
    cout << "NOTYPE";
  }
  if (b) {
    cout << "]" << endl;
  }
}
void printEvent(bool v, Event *event) {
  if (v) {
    cout << "At time "<< event->eventTime << ":" << endl;
    cout << "    " << printEventType(event->eventType) << endl;
    printThread(event->process->processID, event->thread->threadID, event->process->processType);
    if (event->eventType != 7){
      printTransition(event->eventType);
      cout << endl;
    }
  }
}

string printEventType(int n){
  switch(n){
    case 0: return "THREAD_ARRIVED";
    break;
    case 1: return "THREAD_DISPATCH_COMPLETED";
    break;
    case 2: return "PROCESS_DISPATCH_COMPLETED";
    break;
    case 3: return "CPU_BURST_COMPLETED";
    break;
    case 4: return "IO_BURST_COMPLETED";
    break;
    case 5: return "THREAD_COMPLETED";
    break;
    case 6: return "THREAD_PREEMPTED";
    break;
    case 7:
    default: return "DISPATCHER_INVOKED";
  }
}

void printTransition(int n){
  string firstState = "";
  string secondState = "";
  switch (n){
    case 0: firstState = "NEW";
            secondState = "READY";
            break;
    case 1:
    case 2:
            firstState = "READY";
            secondState = "RUNNING";
            break;
    case 3:
            firstState = "RUNNING";
            secondState = "BLOCKED";
            break;
    case 4:
            firstState = "BLOCKED";
            secondState = "READY";
            break;
    case 5:
            firstState = "RUNNING";
            secondState = "EXIT";
            break;
    case 6:
            firstState = "RUNNING";
            secondState = "READY";
            break;
  }
  cout << "    Transitioned from " << firstState << " to " << secondState << endl;
}

void printThreadWiseProcess(Process* process){
  cout << "Process " << process->processID << ":";
  printProcessType(true, process->processType);
  for (Thread *t : process->threads){
    cout << "Thread " << t->threadID << ":   ";
    cout << "ARR: " << left << setw(7) << t->arrivalTime;
    cout << "CPU: " << setw(7) << t->CPUTime;
    cout << "I/O: " << setw(7) << t->IOTime;
    cout << "TRT: " << setw(7) << t->endTime - t->arrivalTime;
    cout << "END: " << setw(7) << t->endTime << endl;
  }
  cout << endl;
}

void printProcessDetails(vector<Thread *>& v, int ptype){
  printProcessType(false, ptype);
  cout << " THREADS:" << endl;
  cout << "    " << setw(22) << left << "Total count:";
  cout << setw(7) << right << v.size() << endl;
  cout << "    " << setw(22) << left << "Avg. response time:";
  cout << setw(7) << right << fixed << setprecision(2) << calculateAvgResponse(v) << endl;
  cout << "    " << setw(22) << left << "Avg. turnaround time:";
  cout << setw(7) << right << fixed << setprecision(2) << calculateAvgTurnaround(v) << endl;
}

double calculateAvgResponse(vector<Thread *>& v){
  if (v.size() == 0){
    return 0;
  }
  double totalResponse = 0;
  for (Thread *t : v){
    totalResponse += t->responseTime;
  }
  return totalResponse / v.size();
}

double calculateAvgTurnaround(vector<Thread *>& v){
  if (v.size() == 0){
    return 0;
  }
  double totalTurnaround = 0;
  for (Thread *t : v){
    totalTurnaround += t->endTime - t->arrivalTime;
  }
  return totalTurnaround / v.size();
}

void groupProcesses(vector<Process>& q){
  vector<Thread *> system;
  vector<Thread *> interactive;
  vector<Thread *> normal;
  vector<Thread *> batch;

  for (Process p: q){
    vector<Thread *>* pushLoc;
    if (p.processType == 3){
      pushLoc = &system;
    } else if (p.processType == 2){
      pushLoc = &interactive;
    } else if (p.processType == 1){
      pushLoc = &normal;
    } else {
      pushLoc = &batch;
    }
    for (Thread * t: p.threads){
      pushLoc->push_back(t);
    }
  }

  printProcessDetails(batch, 0);
  printProcessDetails(normal, 1);
  printProcessDetails(interactive, 2);
  printProcessDetails(system, 3);
}

void fixedPrint(string a, double b, bool i){
  cout << setw(21) << left << a;

  if(!i){
    cout << setw(12) << right << int(b);
  } else {
    cout << setw(11) << right << fixed << setprecision(2) << b << "%";
  }

  cout << endl;
}
