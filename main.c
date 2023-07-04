#include "virtual_disk.h"
#include "virtual_filesystem.h"

int main() {
    VirtualDisk disk;
    initializeVirtualDisk(&disk);

    FileSystemTable fsTable;
    fsTable.fileCount = 0;

    createFile(&disk, &fsTable, "file1.txt", 256);
    createFile(&disk, &fsTable, "file2.txt", 512);
    createFile(&disk, &fsTable, "file3.txt", 128);

    writeToFile(&disk, &fsTable, "file1.txt", "This is the content of file1.");
    writeToFile(&disk, &fsTable, "file2.txt", "This is the content of file2.");
    writeToFile(&disk, &fsTable, "file3.txt", "This is the content of file3.");

    openFile(&disk, &fsTable, "file1.txt");
    openFile(&disk, &fsTable, "file2.txt");
    openFile(&disk, &fsTable, "file3.txt");

    deleteFile(&disk, &fsTable, "file2.txt");

    printFileSystemTable(&fsTable);

    destroyVirtualDisk(&disk);

    return 0;
}