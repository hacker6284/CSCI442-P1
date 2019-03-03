#include <iostream>
#include "output.h"

using namespace std;

void help() {
  cout << "Run with input file as an argument to sum the numbers in the input file." << endl;
  cout << "Flags:" << endl;
  cout << "   " << "-o, --output: write output to file" << endl;
  cout << "   " << "-t, --per_thread: display per-thread information" << endl;
  cout << "   " << "-v, --verbose: display all information about processes" << endl;
}

void printThread(int pid, int tid, int ptype) {
  cout << "\tThread " << tid << " ";
  cout << "in process " << pid << " [";
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
  cout << "]" << endl;
}

void printEvent(Event *event) {
  cout << "At time "<< event->eventTime << ":" << endl;
  cout << "\t" << printEventType(event->eventType) << endl;
  printThread(event->process->processID, event->thread->threadID, event->process->processType);
  if (event->eventType != 7){
    printTransition(event->eventType);
    cout << endl;
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
  cout << "\tTransitioned from " << firstState << " to " << secondState << endl;
}
