#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <functional>
#include "output.h"

using namespace std;

bool allQueuesEmpty(queue<Thread *> (&priorities) [4]){
  for (int i = 0; i < 4; i++){
    if (!priorities[i].empty()){
      return false;
    }
  }
  return true;
}

int sumThreads(queue<Thread *> (&priorities) [4], queue<Thread *> & readyQueue){
  int num = 0;
  num += readyQueue.size();
  for (int i = 0; i < 4; i++){
    num += priorities[i].size();
  }
  return num;
}

void handleEvent(priority_queue<Event, vector<Event>, Event>& priorityQueue, queue<Thread *> (&priorities) [4], queue<Thread *>& readyQueue, map<string, int>& records, int algorithm){

  int newTime;
  int eventType;
  Thread *dispatchThread;
  int quantum = 3;

  int processSwitchOverhead = records["PSO"];
  int threadSwitchOverhead = records["TSO"];
  bool verbose = records["verbose"];

  Event thisEvent = priorityQueue.top();
  priorityQueue.pop();

  //giant switch statement for main logic
  switch(thisEvent.eventType){
    //thread arrived
    case 0: printEvent(verbose, &thisEvent);
            if (algorithm == 2){
              priorities[thisEvent.process->processType].push(thisEvent.thread);

            } else {
              readyQueue.push(thisEvent.thread); //push this thread onto the ready queue
            }
            thisEvent.thread->state = 1; //thread is now ready
            if (records["idle"]) {

              priorityQueue.push(Event(7, thisEvent.eventTime, NULL, NULL)); //invoke the dispatcher
              records["idle"] = false;
            }
            break;
    //thread dispatch completed
    case 1:
    //process dispatch completed
    case 2: printEvent(verbose, &thisEvent);
            dispatchThread = thisEvent.thread;
            dispatchThread->state = 2;
            if (dispatchThread->responseTime == -1){
              dispatchThread->responseTime = thisEvent.eventTime - dispatchThread->arrivalTime;
            }

            if (algorithm == 1 && dispatchThread->bursts.at(0).cpuTime > quantum) {
              newTime = thisEvent.eventTime + quantum;
              records["serviceTime"] += quantum;
              eventType = 6;
              dispatchThread->bursts.at(0).cpuTime -= quantum;
            } else {
              newTime = thisEvent.eventTime + dispatchThread->bursts.at(0).cpuTime;
              records["serviceTime"] += newTime - thisEvent.eventTime;
              eventType = 3;
            }

            priorityQueue.push(Event(eventType, newTime, dispatchThread->parentProcess, dispatchThread));
            break;
    //cpu burst completed
    case 3: if (thisEvent.thread->bursts.at(0).ioTime > 0){
              printEvent(verbose, &thisEvent);
              thisEvent.thread->state = 3;
              newTime = thisEvent.eventTime + thisEvent.thread->bursts.at(0).ioTime;
              records["ioTime"] += newTime - thisEvent.eventTime;
              thisEvent.thread->bursts.erase(thisEvent.thread->bursts.begin());
              priorityQueue.push(Event(4, newTime, thisEvent.thread->parentProcess, thisEvent.thread));
              priorityQueue.push(Event(7, thisEvent.eventTime, thisEvent.thread->parentProcess, thisEvent.thread));
            } else {
              priorityQueue.push(Event(5, thisEvent.eventTime, thisEvent.thread->parentProcess, thisEvent.thread));
              thisEvent.thread->bursts.erase(thisEvent.thread->bursts.begin());
            }
            break;
    //io burst completed
    case 4: printEvent(verbose, &thisEvent);
            thisEvent.thread->state = 1;
            if (algorithm == 2){
              priorities[thisEvent.process->processType].push(thisEvent.thread);
            } else {
              readyQueue.push(thisEvent.thread);
            }
            break;
    //thread completed
    case 5: printEvent(verbose, &thisEvent);
            thisEvent.thread->state = 4;
            thisEvent.thread->endTime = thisEvent.eventTime;
            records["elapsedTime"] = thisEvent.eventTime;
            if (priorityQueue.size() > 0 || readyQueue.size() > 0 || !allQueuesEmpty(priorities)){
              priorityQueue.push(Event(7, thisEvent.eventTime, thisEvent.thread->parentProcess, thisEvent.thread));
            }
            break;
    //thread preempted
    case 6: printEvent(verbose, &thisEvent);
            thisEvent.thread->state = 1;
            readyQueue.push(thisEvent.thread);
            priorityQueue.push(Event(7, thisEvent.eventTime, thisEvent.thread->parentProcess, thisEvent.thread));
            break;
    //dispatcher invoked
    case 7: if (!readyQueue.empty() || !allQueuesEmpty(priorities)){
              cout << "Hey" << endl;
              if (algorithm == 2){
                for (int i = 0; i < 4; i++){
                  if (!priorities[i].empty()){
                    dispatchThread = priorities[i].front();
                    priorities[i].pop();
                    break;
                  }
                }
              } else {
                dispatchThread = readyQueue.front();
                readyQueue.pop();
              }
              if (thisEvent.thread == NULL || thisEvent.thread->parentProcess != dispatchThread->parentProcess){
                priorityQueue.push(Event(2, thisEvent.eventTime + processSwitchOverhead, dispatchThread->parentProcess, dispatchThread));
                records["dispatchTime"] += processSwitchOverhead;
              } else {
                priorityQueue.push(Event(1, thisEvent.eventTime + threadSwitchOverhead, dispatchThread->parentProcess, dispatchThread));
                records["dispatchTime"] += threadSwitchOverhead;
              }
              Event *newEvent = new Event(7, thisEvent.eventTime, dispatchThread->parentProcess, dispatchThread);
              printEvent(verbose, newEvent);
              if (verbose) {
                cout << "    Selected from " << sumThreads(priorities, readyQueue) + 1 << " threads; ";
               if (algorithm == 1 && newEvent->thread->bursts.at(0).cpuTime > quantum) {
                 cout << "alloted time slice of 3" << endl;
               } else {
                 cout << "will run to completion of burst" << endl;
               }
               cout << endl;
              }
            } else {
               priorityQueue.push(Event(7, priorityQueue.top().eventTime, thisEvent.thread->parentProcess, thisEvent.thread));
               records["idleTime"] += priorityQueue.top().eventTime - thisEvent.eventTime;
            }
            break;
  }
}
