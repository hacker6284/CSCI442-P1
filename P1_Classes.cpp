/*******************************************************
 *CSCI442 Project 1 Deliverable 1
 *Author: Zach Mills
 *Date: February 8, 2019
 *******************************************************/

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "P1_Classes.h"

using namespace std;

Burst::Burst(int cpu, int io){
  cpuTime = cpu;
  ioTime = io;
}

Thread::Thread(int arrival, vector<Burst> b, Process *p, int s, int id){
  arrivalTime = arrival;
  bursts = b;
  parentProcess = p;
  state = s;
  threadID = id;
}

Thread::Thread(){
  arrivalTime = 0;
  vector<Burst> myBursts;
  bursts = myBursts;
  parentProcess = NULL;
  state = 0;
  threadID = 0;
}

bool Thread::operator()(Thread *t1, Thread *t2){
  if (t1->parentProcess->processID > t2->parentProcess->processID){
    return true;
  } else if ((t1->parentProcess->processID == t2->parentProcess->processID) && t1->threadID > t2->threadID) {
    return true;
  } else {
    return false;
  }
}

Process::Process(int pid, int type, vector<Thread> t){
  processID = pid;
  processType = type;
  threads = t;
}

Process::Process(){
  processID = 0;
  processType = 0;
  vector<Thread> myThreads;
  threads = myThreads;
}

bool Process::operator()(Process *p1, Process *p2){
  if (p1->processType > p2->processType){
    return true;
  } else {
    return false;
  }
}

Event::Event(){
  eventType = 0;
  eventTime = 0;
  process = NULL;
  thread = NULL;
}

Event::Event(int type, int t, Process *p, Thread *thr){
  eventType = type;
  eventTime = t;
  process = p;
  thread = thr;
}

bool Event::operator()(Event e1, Event e2){
  if (e1.eventTime > e2.eventTime){
    return true;
  }
  return false;
}
