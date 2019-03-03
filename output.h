#include <iostream>
#include <queue>
#include "P1_Classes.h"

using namespace std;

void help();

void printThread(int pid, int tid, int ptype);

void printEvent(bool v, Event *event);

string printEventType(int n);

void printTransition(int n);

void printThreadWiseProcess(Process *process);

void printProcessType(bool b, int ptype);

void printProcessDetails(vector<Thread *>& v, int ptype);

double calculateAvgResponse(vector<Thread *>& v);

double calculateAvgTurnaround(vector<Thread *>& v);

void groupProcesses(vector<Process>& q);

void fixedPrint(string a, double b, bool i);
