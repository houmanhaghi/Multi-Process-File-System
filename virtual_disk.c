#include "virtual_disk.h"
#include <stdio.h>
#include <stdlib.h>

void initializeVirtualDisk(VirtualDisk* disk) {
    disk->diskSize = DISK_SIZE;
    disk->blockSize = BLOCK_SIZE;
    disk->numBlocks = disk->diskSize / disk->blockSize;
    disk->file = fopen("virtual_disk.bin", "wb+");
    disk->blockLocks = (int*)calloc(disk->numBlocks, sizeof(int));
}

void readBlock(VirtualDisk* disk, int blockNumber, char* buffer) {
    if (blockNumber >= 0 && blockNumber < disk->numBlocks) {
        fseek(disk->file, blockNumber * disk->blockSize, SEEK_SET);
        fread(buffer, disk->blockSize, 1, disk->file);
    }
}

void writeBlock(VirtualDisk* disk, int blockNumber, const char* data){
    if (blockNumber >= 0 && blockNumber < disk->numBlocks) {
        fseek(disk->file, blockNumber * disk->blockSize, SEEK_SET);
        fwrite(data, disk->blockSize, 1, disk->file);
        fflush(disk->file);
    }
}

void destroyVirtualDisk(VirtualDisk* disk) {
    fclose(disk->file);
    free(disk->blockLocks);
}