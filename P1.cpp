/*******************************************************
 *CSCI442 Project 1 Deliverable 1
 *Author: Zach Mills
 *Date: February, 2019
 *******************************************************/

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <queue>
#include <map>
#include "Handler.h"

using namespace std;

int main(int argc, char* argv[]) {

  bool verbose = false;
  bool perThread = false;
  int algorithm = 0;  //FCFS
  string algorithm_str = "NONE";

  //Check flags
  switch (argc) {
    case 1:                                     //no arguments provided
      cout << "Not enough arguments" << endl;
      return 1;
      break;
    case 2:                                     //only one provided
      if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        help();
        return 0;
      }
    break;
    default:


    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        if (argv[i][1] == '-') {
          //two dashes case
          if (strcmp(argv[i], "--verbose") == 0){
            verbose = true;
          } else if (strcmp(argv[i], "--per-thread") == 0){
            perThread = true;
          } else if (strcmp(argv[i], "--algorithm") == 0){
            algorithm_str = argv[i+1];
            i++;
        } else {
          cout << "Unknown flag" << endl;
          exit(3);
        }
      } else {
        for (int j = 1; j < strlen(argv[i]); j++){
          if (argv[i][j] == 'v') {
            verbose = true;
          } else if (argv[i][j] == 't') {
            perThread = true;
          } else if (argv[i][j] == 'a') {
            algorithm_str = argv[i+1];
            i++;
            break;
          } else {
            cout << "Unknown flag" << endl;
            exit(4);
          }
        }
      }
    }
  }
  }

if (algorithm_str != "NONE"){
    if (algorithm_str == "FCFS"){
      algorithm = 0;
    } else if (algorithm_str == "RR"){
      algorithm = 1;
    } else if (algorithm_str == "PRIORITY"){
      algorithm = 2;
    } else if (algorithm_str == "CUSTOM"){
      algorithm = 3;
    } else {
      cout << "Unknown Algorithm" << endl;
      exit(2);
    }
  }

  //open the file for reading
  fstream fin;
  fin.open(argv[argc - 1], fstream::in);
  if (fin.fail()) {                     //error if file failed to open
    cout << "Invalid input file. Run with -h for instructions." << endl;
    return 3;
  }

  //vector<Event> priorityQueue; //establish priority queue
  priority_queue<Event, vector<Event>, Event> priorityQueue;
  queue<Thread*> readyQueue;  //establish ready queue

  //establish priority-based ready queues.
  queue<Thread *> priorities[4];
  for (int i = 0; i < 4; i++){
    queue<Thread *> tempQueue;
    priorities[i] = tempQueue;
  }

  //create queues for threadwise output
  priority_queue<Process *, vector<Process *>, Process> threadWise;

  //read in initial values
  int numProcesses, threadSwitchOverhead, processSwitchOverhead;
  fin >> numProcesses;
  fin >> threadSwitchOverhead;
  fin >> processSwitchOverhead;

  int processID, processType, numThreads;
  vector<Process> processes;
  Process *newProcess;
  Thread *newThread;
  for (int i = 0; i < numProcesses; i++){
    newProcess = new Process();
    fin >> processID;
    fin >> processType;
    fin >> numThreads;

    int arrivalTime, numBursts;
    vector<Thread *> process;
    newProcess->processID = processID;
    newProcess->processType = processType;
    newProcess->threads = process;
    for (int j = 0; j < numThreads; j++){
      newThread = new Thread();
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
      newThread->arrivalTime = arrivalTime;
      newThread->bursts = thread;
      newThread->parentProcess = newProcess;
      newThread->state = 0;
      newThread->threadID = j;
      newThread->sumTimes();
      newProcess->threads.push_back(newThread);
      priorityQueue.push(Event(0, arrivalTime, newProcess, newThread));
    }
    processes.push_back(*(newProcess));
    threadWise.push(newProcess);
  }

  //close the stream
  fin.close();

  //set beginning state
  bool idle = true;

  //set integers for record-keeping
  map<string, int> records;
  records["elapsedTime"] = 0;
  records["serviceTime"] = 0;
  records["ioTime"] = 0;
  records["idleTime"] = 0;
  records["dispatchTime"] = 0;
  records["verbose"] = int(verbose);
  records["PSO"] = processSwitchOverhead;
  records["TSO"] = threadSwitchOverhead;
  records["idle"] = int(idle);

  //simulate events
  while (!priorityQueue.empty()){
    handleEvent(priorityQueue, priorities, readyQueue, records, algorithm);
  }

  //output threadwise information
  while (perThread && !threadWise.empty()) {
    printThreadWiseProcess(threadWise.top());
    threadWise.pop();
  }

  cout << "SIMULATION COMPLETED!" << endl << endl;

  groupProcesses(processes); //prints process details

  cout << endl;

  fixedPrint("Total elapsed time: ", records["elapsedTime"], false);
  fixedPrint("Total service time: ", records["serviceTime"], false);
  fixedPrint("Total I/O time: ", records["ioTime"], false);
  fixedPrint("Total dispatch time: ", records["dispatchTime"], false);
  fixedPrint("Total idle time: ", records["idleTime"], false);
  cout << endl;
  fixedPrint("CPU utilization: ", 100 * double(records["elapsedTime"] - records["idleTime"]) / double(records["elapsedTime"]), true);
  fixedPrint("CPU efficiency: ", 100 * double(records["serviceTime"]) / double(records["elapsedTime"]), true);

  return 0;
}
