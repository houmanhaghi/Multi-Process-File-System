#ifndef PROCESS_MANAGEMENT_H
#define PROCESS_MANAGEMENT_H

#define MAX_PROCESSES 100 // Maximum number of processes

typedef struct {
    int processID;
    // Add other attributes as needed (e.g., file access permissions, status)
} Process;

Process* createProcess();
void destroyProcess(Process* process);

#define MAX_ACTIVE_PROCESSES 10 // Maximum number of active processes

void addProcessToActiveList(Process* process);
void removeProcessFromActiveList(Process* process);

void terminateProcess(Process* process);

Process* getNextProcess();

#endif  /* PROCESS_MANAGEMENT_H */