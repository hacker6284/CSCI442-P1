#include <iostream>
#include <queue>
#include "P1_Classes.h"

using namespace std;

void help();

void printThread(int pid, int tid, int ptype);

void printEvent(bool v, Event *event);

string printEventType(int n);

void printTransition(int n);

void printThreadWiseProcess(Process* process);

void processType(int ptype);
