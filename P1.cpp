/*******************************************************
 *CSCI442 Project 1 Deliverable 1
 *Author: Zach Mills
 *Date: February, 2019
 *******************************************************/

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "output.h"
#include <algorithm>
#include <functional>
#include <queue>

using namespace std;

int main(int argc, char* argv[]) {

  bool verbose = false;
  bool perThread = false;

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
      if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
        help();
        return 0;
      } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
        verbose = true;
      } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--per_thread") == 0) {
        perThread = true;
      } else if (strcmp(argv[i], "-tv") == 0 || strcmp(argv[i], "-vt") == 0) {
        perThread = true;
        verbose = true;
      }
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

  //create queues for threadwise output
  priority_queue<Process *, vector<Process *>, Process> typeSort;
  priority_queue<Thread *, vector<Thread *>, Thread> threadWise;

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
    vector<Thread> process;
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
      newProcess->threads.push_back(*(newThread));
      threadWise.push(newThread);
      priorityQueue.push(Event(0, arrivalTime, newProcess, newThread));
    }
    processes.push_back(*(newProcess));
    typeSort.push(newProcess);
  }

  //close the stream
  fin.close();

  //set beginning state
  bool idle = true;

  //output events
  while (!priorityQueue.empty()){
    Event thisEvent = priorityQueue.top();
    priorityQueue.pop();

    int newTime;
    Thread *dispatchThread;

    //giant switch statement for main logic
    switch(thisEvent.eventType){
      case 0: printEvent(verbose, &thisEvent);
              readyQueue.push(thisEvent.thread); //push this thread onto the ready queue
              thisEvent.thread->state = 1; //thread is now ready
              if (idle) {
                priorityQueue.push(Event(7, thisEvent.eventTime, NULL, NULL)); //invoke the dispatcher
                idle = false;
              }
              break;
      case 1:
      case 2: printEvent(verbose, &thisEvent);
              dispatchThread = readyQueue.front();
              readyQueue.pop();
              dispatchThread->state = 2;
              newTime = thisEvent.eventTime + dispatchThread->bursts.at(0).cpuTime;
              priorityQueue.push(Event(3, newTime, dispatchThread->parentProcess, dispatchThread));
              break;
      case 3: if (thisEvent.thread->bursts.at(0).ioTime > 0){
                printEvent(verbose, &thisEvent);
                thisEvent.thread->state = 3;
                newTime = thisEvent.eventTime + thisEvent.thread->bursts.at(0).ioTime;
                thisEvent.thread->bursts.erase(thisEvent.thread->bursts.begin());
                priorityQueue.push(Event(4, newTime, thisEvent.thread->parentProcess, thisEvent.thread));
                priorityQueue.push(Event(7, thisEvent.eventTime, thisEvent.thread->parentProcess, thisEvent.thread));
              } else {
                priorityQueue.push(Event(5, thisEvent.eventTime, thisEvent.thread->parentProcess, thisEvent.thread));
                thisEvent.thread->bursts.erase(thisEvent.thread->bursts.begin());
              }
              break;
      case 4: printEvent(verbose, &thisEvent);
              thisEvent.thread->state = 1;
              readyQueue.push(thisEvent.thread);
              break;
      case 5: printEvent(verbose, &thisEvent);
              thisEvent.thread->state = 4;
              if (priorityQueue.size() > 0 || readyQueue.size() > 0){
                priorityQueue.push(Event(7, thisEvent.eventTime, thisEvent.thread->parentProcess, thisEvent.thread));
              }
              break;
      case 6: printEvent(verbose, &thisEvent);
              break;
      case 7: if (readyQueue.size() > 0){
               if (thisEvent.thread == NULL || thisEvent.thread->parentProcess != readyQueue.front()->parentProcess){
                 priorityQueue.push(Event(2, thisEvent.eventTime + processSwitchOverhead, readyQueue.front()->parentProcess, readyQueue.front()));
               } else {
                 priorityQueue.push(Event(1, thisEvent.eventTime + threadSwitchOverhead, readyQueue.front()->parentProcess, readyQueue.front()));
               }
               Event *newEvent = new Event(7, thisEvent.eventTime, readyQueue.front()->parentProcess, readyQueue.front());
               printEvent(verbose, newEvent);
               cout << "\tSelected from " << readyQueue.size() << " threads; will run to completion of burst" << endl;
               cout << endl;
              } else {
               priorityQueue.push(Event(7, priorityQueue.top().eventTime, thisEvent.thread->parentProcess, thisEvent.thread));
              }
              break;
    }
  }

  while (!typeSort.empty()) {
    Process * printProcess = typeSort.top();
    cout << printProcess->processType << " : " << printProcess->threads.size() << endl;
    typeSort.pop();
  }

  while (!threadWise.empty()) {
    Thread * printThread = threadWise.top();
    cout << printThread->parentProcess->processID << " : " << printThread->threadID << " : " << printThread->parentProcess->processType << endl;
    threadWise.pop();
  }


  return 0;
}
