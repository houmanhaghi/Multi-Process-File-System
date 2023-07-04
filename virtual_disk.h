#include <stdio.h>
#ifndef VIRTUAL_DISK_H
#define VIRTUAL_DISK_H
#define DISK_SIZE 1024
#define BLOCK_SIZE 64

typedef struct {
    int diskSize;
    int blockSize;
    int numBlocks;
    FILE* file;
    int* blockLocks;
} VirtualDisk;

void initializeVirtualDisk(VirtualDisk* disk);
void readBlock(VirtualDisk* disk, int blockNumber, char* buffer);
void writeBlock(VirtualDisk* disk, int blockNumber, const char* data);
void destroyVirtualDisk(VirtualDisk* disk);

#endif  // VIRTUAL_DISK_H