#include "virtual_filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void createFile(VirtualDisk* disk, FileSystemTable* fsTable, const char* filename, int size) {
    if (fsTable->fileCount >= MAX_FILES) {
        printf("Maximum file count reached.\n");
        return;
    }

    if (strlen(filename) >= MAX_FILENAME_LENGTH) {
        printf("Filename is too long.\n");
        return;
    }

    int blockCount = (size + disk->blockSize - 1) / disk->blockSize;
    int blockStart = -1;
    int i, j;
    for (i = 0; i < disk->numBlocks - blockCount + 1; i++) {
        int available = 1;
        for (j = i; j < i + blockCount; j++) {
            if (disk->blockLocks[j]) {
                available = 0;
                break;
            }
        }
        if (available) {
            blockStart = i;
            break;
        }
    }

    if (blockStart == -1) {
        printf("Not enough free blocks to create the file.\n");
        return;
    }

    strncpy(fsTable->metadata[fsTable->fileCount].filename, filename, MAX_FILENAME_LENGTH);
    fsTable->metadata[fsTable->fileCount].size = size;
    fsTable->metadata[fsTable->fileCount].blockStart = blockStart;
       for (int i = 0; i < MAX_PROCESSES; i++) {
    fsTable->metadata[fsTable->fileCount].processPermissions[i].readPermission = 1;
    fsTable->metadata[fsTable->fileCount].processPermissions[i].writePermission = 1;

}
    fsTable->fileCount++;

 

    for (i = blockStart; i < blockStart + blockCount; i++) {
        disk->blockLocks[i] = 1;
    }

    printf("File '%s' created successfully.\n", filename);
}

void deleteFile(VirtualDisk* disk, FileSystemTable* fsTable, const char* filename) {
    int i, j;
    for (i = 0; i < fsTable->fileCount; i++) {
        if (strcmp(fsTable->metadata[i].filename, filename) == 0) {
            int blockStart = fsTable->metadata[i].blockStart;
            int blockCount = (fsTable->metadata[i].size + disk->blockSize - 1) / disk->blockSize;
            for (j = blockStart; j < blockStart + blockCount; j++) {
                disk->blockLocks[j] = 0;
            }
            memmove(&fsTable->metadata[i], &fsTable->metadata[i + 1], (fsTable->fileCount - i - 1) * sizeof(FileMetadata));
            fsTable->fileCount--;
            printf("File '%s' deleted successfully.\n", filename);
            return;
        }
    }
    printf("File '%s' not found.\n", filename);
}

void writeToFile(VirtualDisk* disk, FileSystemTable* fsTable, const char* filename, const char* data) {
    int i;
    for (i = 0; i < fsTable->fileCount; i++) {
        if (strcmp(fsTable->metadata[i].filename, filename) == 0) {
            int blockStart = fsTable->metadata[i].blockStart;
            int blockCount = (fsTable->metadata[i].size + disk->blockSize - 1) / disk->blockSize;
            int j;
            for (j = 0; j < blockCount; j++) {
                int blockNumber = blockStart + j;
                writeBlock(disk, blockNumber, data + j * disk->blockSize);
            }
            printf("Data written to file '%s'.\n", filename);
            return;
        }
    }
    printf("File '%s' not found.\n", filename);
}

void openFile(VirtualDisk* disk, FileSystemTable* fsTable, const char* filename) {
    int i;
    for (i = 0; i < fsTable->fileCount; i++) {
        if (strcmp(fsTable->metadata[i].filename, filename) == 0) {
            int blockStart = fsTable->metadata[i].blockStart;
            int blockCount = (fsTable->metadata[i].size + disk->blockSize - 1) / disk->blockSize;
            int j;
            for (j = 0; j < blockCount; j++) {
                int blockNumber = blockStart + j;
                char buffer[disk->blockSize];
                readBlock(disk, blockNumber, buffer);
                printf("%s", buffer);
            }
            printf("\n");
            return;
        }
    }
    printf("File '%s' not found.\n", filename);
}

void printFileSystemTable(FileSystemTable* fsTable) {
    printf("File System Table:\n");
    printf("==================\n");
    int i;
    for (i = 0; i < fsTable->fileCount; i++) {
        printf("Filename: %s, Size: %d, Block Start: %d\n", fsTable->metadata[i].filename,
               fsTable->metadata[i].size, fsTable->metadata[i].blockStart);
    }
}


void setFilePermissionsForProcess(VirtualDisk* disk, FileSystemTable* fsTable, const char* filename, int processID, FilePermissions permissions) {
    // Search for the process ID in the processPermissions table and update its permissions
     int i;
    for (i = 0; i < fsTable->fileCount; i++) {
        if (strcmp(fsTable->metadata[i].filename, filename) == 0) {
        fsTable->metadata[i].processPermissions[processID] = permissions;
        }
    }
        
    
    printf("File '%s' not found.\n", filename);
}

FilePermissions getFilePermissionsForProcess(VirtualDisk* disk, FileSystemTable* fsTable, const char* filename, int processID) {
    // Retrieve the permissions for the given process ID from the processPermissions table
    
   
    int i;
    for (i = 0; i < fsTable->fileCount; i++) {
        if (strcmp(fsTable->metadata[i].filename, filename) == 0) {
           return fsTable->metadata[i].processPermissions[processID];

        }
    }
    printf("File '%s' not found.\n", filename);
}

