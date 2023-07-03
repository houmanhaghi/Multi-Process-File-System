#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <unistd.h>

struct statvfs vfs_stat;

char all_dirs[256][256] = 
    {"/root/bin/","/root/dev/","/root/etc/","/root/home/","/root/mnt/",
    "/root/proc/","/root/tmp/","/root/usr/"};


void update_meta()
{
    char path[256]; // PATH_MAX is a constant defined in <limits.h> that represents the maximum length of a path on the system.
    if (getcwd(path, sizeof(path)) != NULL) {
    } else {
        perror("getcwd error");
        exit(1);
    }

    int index = 0;
    while(strcmp(all_dirs[index], "\0") != 0)
    {
        FILE *file;
        char directory_path[256] = "";
        strcat(directory_path, path);
        strcat(directory_path, all_dirs[index]);
        char  meta_path[256] = "";
        strcat(meta_path ,directory_path);
        strcat(meta_path, ".meta");


        // Get the file system statistics using statvfs
        if (statvfs(directory_path, &vfs_stat) == -1) {
            perror("Error getting file system statistics");
            exit(1);
        }

        file = fopen(meta_path, "w");
        
        // Check if the file was opened successfully
        if (file == NULL) {
        perror("Error opening the file");
        exit(1);
    }
        // Print directory metadata
        fprintf(file, "Directory: %s\n", directory_path);
        fprintf(file, "Total size: %lu bytes\n", vfs_stat.f_frsize * vfs_stat.f_blocks);
        fprintf(file, "Free space: %lu bytes\n", vfs_stat.f_frsize * vfs_stat.f_bfree );
        fprintf(file, "Number of files: %lu\n", vfs_stat.f_files);

        fclose(file);
        index++;
    }
    
}



int main()
{
    update_meta();



    return 0;
}