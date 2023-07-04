#include "process_management.h"
#include <stdlib.h> // For dynamic memory allocation

Process* createProcess() {
    static int nextProcessID = 1; // Initialize the next process ID to 1
    if (nextProcessID > MAX_PROCESSES) {
        // Handle maximum process limit reached
        return NULL;
    }
    
    Process* newProcess = (Process*) malloc(sizeof(Process));
    if (newProcess == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    
    newProcess->processID = nextProcessID;
    // Initialize other process attributes as needed
    
    nextProcessID++; // Increment the next process ID for the next creation
    
    return newProcess;
}

void destroyProcess(Process* process) {
    free(process);
}

Process* activeProcesses[MAX_ACTIVE_PROCESSES];
int numActiveProcesses = 0;

void addProcessToActiveList(Process* process) {
    if (numActiveProcesses >= MAX_ACTIVE_PROCESSES) {
        // Handle maximum active process limit reached
        return;
    }
    
    activeProcesses[numActiveProcesses] = process;
    numActiveProcesses++;
}

void removeProcessFromActiveList(Process* process) {
    int i;
    for (i = 0; i < numActiveProcesses; i++) {
        if (activeProcesses[i] == process) {
            activeProcesses[i] = activeProcesses[numActiveProcesses - 1];
            numActiveProcesses--;
            break;
        }
    }
}

void terminateProcess(Process* process) {
    // Clean up any resources associated with the process
    // Update the file system or other relevant components
    
    removeProcessFromActiveList(process);
    destroyProcess(process);
}

Process* getNextProcess() {
    // Implement your process scheduling algorithm here
    // Return the next process to be executed based on your scheduling policy
    
    // Round-robin scheduling algorithm:
    // Rotate the active process list to select the next process
    
    if (numActiveProcesses > 0) {
        Process* nextProcess = activeProcesses[0]; // Get the first process in the list
        // Rotate the active process list
        for (int i = 0; i < numActiveProcesses - 1; i++) {
            activeProcesses[i] = activeProcesses[i + 1];
        }
        activeProcesses[numActiveProcesses - 1] = nextProcess; // Move the first process to the end
        
        return nextProcess;
    }
    
    return NULL; // Return NULL if no processes are active
}