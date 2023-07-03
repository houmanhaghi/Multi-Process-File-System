#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


#define MAX_LINE_LENGTH 256
#define STRING_ARRAY_SIZE 256
#define CONFIG "configs.txt"
#define len(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define MAX_ARRAY_SIZE 100
#define MAX_STRING_LENGTH 50
#define MAX_FOLDER_SIZE_BYTES 1000000000 // Set your desired folder size limit in bytes here (e.g., 1GB).


struct statvfs vfs_stat;



char* strip(char* str) {
    // Find the first non-whitespace character
    while (isspace((unsigned char)*str))
        str++;

    if (*str == '\0') // String contains only whitespace
        return str;

    // Find the last non-whitespace character
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Null-terminate the string at the last non-whitespace character
    *(end + 1) = '\0';

    return str;
}


int countFilesRecursively(const char* path) {
    int count = 0;
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip current and parent directories
        }

        char entryPath[512];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", path, entry->d_name);

        struct stat statBuf;
        if (stat(entryPath, &statBuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statBuf.st_mode)) {
            count += countFilesRecursively(entryPath); // Recursively count files in sub-directory
        } else if (S_ISREG(statBuf.st_mode)) {
            count++; // Increment count if it's a regular file
        }
    }

    closedir(dir);
    return count;
}


long int getFolderSize(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        return -1;
    }

    if (S_ISREG(statbuf.st_mode)) {
        return statbuf.st_size;
    }

    DIR *dir = opendir(path);
    if (!dir) {
        return -1;
    }

    long long int total_size = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char child_path[1024];
            snprintf(child_path, sizeof(child_path), "%s/%s", path, entry->d_name);
            total_size += getFolderSize(child_path);
        }
    }
    closedir(dir);

    return total_size;
}




void update_meta()
{
    char path[STRING_ARRAY_SIZE]; 
    if (getcwd(path, sizeof(path)) != NULL) {
    } else {
        perror("getcwd error");
        exit(1);
    }

    int index = 0;
    while(strcmp(meta[index].dir_path, "\0") != 0)
    {
        FILE *file;
        char directory_path[STRING_ARRAY_SIZE] = "";
        strcat(directory_path, path);
        strcat(directory_path, meta[index].dir_path);
        char meta_path[STRING_ARRAY_SIZE] = "";
        strcat(meta_path ,directory_path);
        strcat(meta_path, ".meta");


        file = fopen(meta_path, "w");
        
        // Check if the file was opened successfully
        if (file == NULL) {
        perror("Error opening the file");
        exit(1);
        }

        long int current_size = getFolderSize(directory_path);
        if (current_size == -1) {
            printf("Error: Unable to get the folder size.\n");
            exit(1);
        }


        // Print directory metadata
        fprintf(file, "Directory: %s\n", directory_path);

        // fprintf(file, "Total size: %lu bytes\n", vfs_stat.f_frsize * vfs_stat.f_blocks);

        fprintf(file, "Total size: %lu bytes\n", meta[index].total_size);

        fprintf(file, "Free space: %lu bytes\n", meta[index].total_size - current_size );
        meta[index].free_size = meta[index].total_size - current_size;

        fprintf(file, "Number of files: %d\n", countFilesRecursively(directory_path)-1);
        meta[index].number_of_files = countFilesRecursively(directory_path)-1;

        fclose(file);
        index++;
    }
    
}




// implement function to do CRUD tasks
void create_file()
{

}


void read_file()
{

}


void update_file()
{

}


void delete_file()
{

}


int main()
{
    // update meta date of each directory
    update_meta();


    // read commands from configs.txt
    FILE *file;
    char line[MAX_LINE_LENGTH][STRING_ARRAY_SIZE];
    char config_path[STRING_ARRAY_SIZE]; 
    if (getcwd(config_path, sizeof(config_path)) != NULL) {
        strcat(config_path, "/src/");
        strcat(config_path, CONFIG);
    } else {
        perror("getcwd error");
        exit(1);
    }

    file = fopen(config_path, "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }
    
    // Read lines from the file until the end
    int lineIndex = 0;
    while (fgets(line[lineIndex], sizeof(line[lineIndex]), file) != NULL) {
        if (strcmp(line[lineIndex], "") > 0)
            lineIndex++;
    }
    fclose(file);


    // create separate process for each line of congigs
    for (int p = 0; p < lineIndex; p++) {
        // Separate each command
        char commands[STRING_ARRAY_SIZE][STRING_ARRAY_SIZE] = {};
        const char delimiter[] = ",";

        // Strip leading and trailing whitespace from the line
        char* stripped_line = strip(line[p]);

        // Tokenize the stripped_line and store tokens in commands array
        char* token = strtok(stripped_line, delimiter);
        int commands_count = 0; // Initialize to 0 since no tokens are found yet
        char token_buffer[STRING_ARRAY_SIZE];

        int i = 0;
        while (token != NULL && i < STRING_ARRAY_SIZE) {
            char* stripped_token = strip(token); // Strip each token individually
            strcpy(token_buffer, stripped_token);
            strcpy(commands[i], token_buffer);
            token = strtok(NULL, delimiter);
            i++;
            commands_count++;
        }

        // Print the commands stored in the commands array
        // for (int a = 0; a < i; a++) {
        //     printf("%s\n", commands[a]);
        // }
    }


    return 0;
}