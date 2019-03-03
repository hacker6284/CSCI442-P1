/*******************************************************
 *CSCI442 Project 1 Deliverable 1
 *Author: Zach Mills
 *Date: February 8, 2019
 *******************************************************/

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>

using namespace std;

class Process;

class Burst{
public:
  int cpuTime;
  int ioTime;
  Burst(int cpu, int io);
};

class Thread{
public:
  int threadID;
  int state;
  int arrivalTime;
  vector<Burst> bursts;
  Process *parentProcess;
  //timekeeping stuff
  int endTime;
  int CPUTime;
  int IOTime;
  int responseTime;

  Thread(int arrival, vector<Burst> b, Process *p, int s, int id);
  Thread();
  void sumTimes();
};

class Process{
public:
  int processID;
  int processType;
  vector<Thread *> threads;
  Process(int pid, int type, vector<Thread *> t);
  Process();
  bool operator()(Process *p1, Process *p2);
};

class Event{
public:
  Process *process;
  Thread *thread;
  int eventType;
  int eventTime;
  Event(int type, int t, Process *p, Thread *thr);
  Event();
  bool operator()(Event e1, Event e2);
};
