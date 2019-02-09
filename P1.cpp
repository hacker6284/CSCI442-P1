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
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {

  //open the file for reading
  fstream fin;
  fin.open(argv[argc - 1], fstream::in);
  if (fin.fail()) {                     //error if file failed to open
    cout << "Invalid input file. Run with -h for instructions." << endl;
    return 3;
  }

  vector<Event> priorityQueue; //establish priority queue

  //read in initial values
  int numProcesses, threadSwitchOverhead, processSwitchOverhead;
  fin >> numProcesses;
  fin >> threadSwitchOverhead;
  fin >> processSwitchOverhead;

  int processID, processType, numThreads;
  vector<Process> processes;
  for (int i = 0; i < numProcesses; i++){
    fin >> processID;
    fin >> processType;
    fin >> numThreads;

    int arrivalTime, numBursts;
    vector<Thread> process;
    for (int j = 0; j < numThreads; j++){
      fin >> arrivalTime;
      fin >> numBursts;

      int cpuTime, ioTime;
      vector<Burst> thread;
      for (int k = 0; k < numBursts - 1; k++){
        fin >> cpuTime;
        fin >> ioTime;
        thread.push_back(Burst(cpuTime, ioTime));
      }
      fin >> cpuTime;
      thread.push_back(Burst(cpuTime, 0));
      process.push_back(Thread(arrivalTime, thread));
      priorityQueue.push_back(Event(0, arrivalTime));
    }
    processes.push_back(Process(processID, processType, process));
  }

  //sort queue
  sort(priorityQueue.begin(), priorityQueue.end(), compareEvents);

  //output events
  for (int i = 0; i < priorityQueue.size(); i++){
    cout << "THREAD_ARRIVED at " << priorityQueue.at(i).eventTime << " seconds" << endl;
  }

  //close the stream
  fin.close();


  return 0;
}
