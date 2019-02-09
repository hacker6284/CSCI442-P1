/*******************************************************
 *CSCI442 Project 1 Deliverable 1
 *Author: Zach Mills
 *Date: October 1, 2018
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

Thread::Thread(int arrival, vector<Burst> b){
  arrivalTime = arrival;
  bursts = b;
}

Process::Process(int pid, int type, vector<Thread> t){
  processID = pid;
  processType = type;
  threads = t;
}

Event::Event(){
  eventType = 0;
  eventTime = 0;
}

Event::Event(int type, int t){
  eventType = type;
  eventTime = t;
}

bool Event::operator()(Event e1, Event e2){
  if (e1.eventTime > e2.eventTime){
    return true;
  }
  return false;
}
