#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <functional>
#include "output.h"

using namespace std;

bool allQueuesEmpty(queue<Thread *> (&priorities) [4]);

int sumThreads(queue<Thread *> (&priorities) [4], queue<Thread *> & readyQueue);

void handleEvent(priority_queue<Event, vector<Event>, Event>& priorityQueue, queue<Thread *> (&priorities) [4], queue<Thread *>& readyQueue, map<string, int>& records, int algorithm);
