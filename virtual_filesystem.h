#ifndef VIRTUAL_FILESYSTEM_H
#define VIRTUAL_FILESYSTEM_H

#include "virtual_disk.h"
#include "process_management.h"
#define MAX_FILENAME_LENGTH 100
#define MAX_FILES 100

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    int size;
    int blockStart; 
    FilePermissions processPermissions[MAX_PROCESSES];   
} FileMetadata;

typedef struct {
    int readPermission;
    int writePermission;
} FilePermissions;

typedef struct {
    int fileCount;
    FileMetadata metadata[MAX_FILES];
} FileSystemTable;

void createFile(VirtualDisk* disk, FileSystemTable* fsTable, const char* filename, int size);
void deleteFile(VirtualDisk* disk, FileSystemTable* fsTable, const char* filename);
void writeToFile(VirtualDisk* disk, FileSystemTable* fsTable, const char* filename, const char* data);
void openFile(VirtualDisk* disk, FileSystemTable* fsTable, const char* filename);
void printFileSystemTable(FileSystemTable* fsTable);

#endif  // VIRTUAL_FILESYSTEM_H