README
======

CSCI442 Project 1: CPU Scheduling Simulator
-------------------------------------------
Author: Zach Mills
------------------

Contents:
---------
1. *README* - This readme
2. *Makefile* - Makefile for the project
3. *P1.cpp* - main c++ file, containing the main function
4. *P1_Classes.h* - header file for the class structures needed for the project
5. *P1_Classes.cpp* - definitions of classes declared in the header
6. *Handler.h* - header file for the event handler process, and a few helper functions
7. *Handler.cpp* - implementation of the Handler declared in the header file, and the helper functions
8. *output.h* - header file for functions to format output
9. *output.cpp* - implementation of output format functions declared in the header


Custom Algorithm:
-----------------

Threads are admitted into a ready queue as they arrive. The ready queue is a normal queue. Threads will be read from the queue in the order they arrived. Initially, we will begin processing the first Thread to have arrived. Threads will be assigned time quanta based on their priority (5 - type), and upon expiration of that time slice, they will be placed into 4 separate ready queues based on priority. If instead they complete their cpu burst, they will block on I/O, and be returned to the original ready queue once their I/O burst completes. When the dispatcher requires a new thread to execute, it will choose the first thread in the ready queue only if it only requires a thread switch and not a full process switch. Otherwise, it will choose the highest priority thread available from the 4 priority-based queues. If all 4 are empty, it will choose from the ready queue after all. This algorithm attempts to provide priority preference in two ways, but also minimize overhead from process switching. It’s experimental to say the least. Starvation should not be possible in my algorithm due to the queue structure of the ready queue. All processes should get pushed to the front eventually.

Approximate time spent on project: **40 hours**
