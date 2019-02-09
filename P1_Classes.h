/*******************************************************
 *CSCI442 Project 1 Deliverable 1
 *Author: Zach Mills
 *Date: October 1, 2018
 *******************************************************/

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>

using namespace std;

class Burst{
public:
  int cpuTime;
  int ioTime;
  Burst(int cpu, int io);
};

class Thread{
public:
  int arrivalTime;
  vector<Burst> bursts;
  Thread(int arrival, vector<Burst> b);
};

class Process{
public:
  int processID;
  int processType;
  vector<Thread> threads;
  Process(int pid, int type, vector<Thread> t);
};

class Event{
public:
  int eventType;
  int eventTime;
  Event(int type, int t);
  Event();
  bool operator()(Event e1, Event e2);
};
