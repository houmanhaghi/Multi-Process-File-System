#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif


#define MAX_LINE_LENGTH 256
#define STRING_ARRAY_SIZE 256
#define CONFIG "configs.txt"
#define len(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define MAX_ARRAY_SIZE 100
#define MAX_STRING_LENGTH 50
#define MAX_FOLDER_SIZE_BYTES 1000000000 // Set your desired folder size limit in bytes here (e.g., 1GB).


struct statvfs vfs_stat;


struct permissions {
    char read[MAX_ARRAY_SIZE][MAX_STRING_LENGTH];
    char write[MAX_ARRAY_SIZE][MAX_STRING_LENGTH];
};


struct meta_data {
    char dir_path[STRING_ARRAY_SIZE];
    long int total_size;
    long int free_size;
    long int number_of_files;
    struct permissions permission;
};



struct meta_data meta[9] ={
    {
        .dir_path = "/root/",
        .total_size = 100000000,
        .free_size = 0,
        .number_of_files = 8,
        .permission = {
            .read = {"t0", "t1", "t2", "t3", "t4", "t5"},
            .write = {"t0", "t1", "t2", "t3", "t4"}
        }
    },
    {
        .dir_path = "/root/bin/",
        .total_size = 50000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3", "t4", "t5"},
            .write = {"t0", "t1", "t2", "t3"}
        }
    },
    {
        .dir_path = "/root/dev/",
        .total_size = 20000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3", "t4", "t5"},
            .write = {"t4", "t5"}
        }
    },
    {
        .dir_path = "/root/etc/",
        .total_size = 2000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3", "t4", "t5"},
            .write = {"t0", "t1", "t2"}
        }
    },
    {
        .dir_path = "/root/home/",
        .total_size = 100000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3", "t4", "t5"},
            .write = {"t1", "t2", "t3"}
        }
    },
    {
        .dir_path = "/root/mnt/",
        .total_size = 3000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3"},
            .write = {"t2", "t3"}
        }
    },
    {
        .dir_path = "/root/proc/",
        .total_size = 5000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t4", "t5"},
            .write = {"t0", "t1", "t4"}
        }
    },
    {
        .dir_path = "/root/tmp/",
        .total_size = 25000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3"},
            .write = {"t0", "t1"}
        }
    },
    {
        .dir_path = "/root/usr/",
        .total_size = 40000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1"},
            .write = {"t0"}
        }
    }
};


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


void create_directory(const char* path) {
    // Recursively create directories for the given path
    char* p = strdup(path);
    for (char* sep = strchr(p + 1, '/'); sep; sep = strchr(sep + 1, '/')) {
        *sep = '\0';
#ifdef _WIN32
        mkdir(p);
#else
        mkdir(p, 0766); // Unix-style permissions
#endif
        *sep = '/';
    }
    free(p);
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
void create_file(char* path, char* fileName, char* write_string )
{
    if (strcmp(write_string, "\"\"") == 0)
        write_string = "";

    FILE *file;
    char fullPath[STRING_ARRAY_SIZE];
    if (getcwd(fullPath, sizeof(fullPath)) != NULL) {
        strcat(fullPath, "/");
        strcat(fullPath, (char*) path);
        create_directory(fullPath);
        strcat(fullPath, (char*) fileName);
    } else {
        perror("getcwd error");
        exit(1);
    }

    file = fopen(fullPath, "w");
    if (file == NULL) {
        perror("Error opening the file");
        printf("%s", fullPath);
        exit(1);
    }

    fprintf(file, "%s\n", (char *)write_string);

    fclose(file);

}


void read_file(char* path, char* fileName)
{
    FILE *file;
    char fullPath[STRING_ARRAY_SIZE];
    if (getcwd(fullPath, sizeof(fullPath)) != NULL) {
        strcat(fullPath, "/");
        strcat(fullPath, (char*) path);
        strcat(fullPath, (char*) fileName);
    } else {
        perror("getcwd error");
        exit(1);
    }

    file = fopen(fullPath, "r");
    if (file == NULL) {
        perror("Error opening the file");
        printf("%s", fullPath);
        exit(1);
    }

    char buffer[STRING_ARRAY_SIZE];
    while (fscanf(file, "%99[^\n]\n", buffer) == 1) {
        // Process the line read from the file
        printf("%s\n", buffer);
    }

    fclose(file);
}


void update_file(char* path, char* fileName, char* append_string )
{
    if (strcmp(append_string, "\"\"") == 0)
        append_string = "";

    // append file in here
    FILE *file;
    char fullPath[STRING_ARRAY_SIZE];
    if (getcwd(fullPath, sizeof(fullPath)) != NULL) {
        strcat(fullPath, "/");
        strcat(fullPath, (char*) path);
        strcat(fullPath, (char*) fileName);
    } else {
        perror("getcwd error");
        exit(1);
    }

    file = fopen(fullPath, "a");
    if (file == NULL) {
        perror("Error opening the file");
        printf("%s", fullPath);
        exit(1);
    }

    fprintf(file, "%s\n", (char *)append_string);

    fclose(file);

}


void delete_file(char* path, char* fileName)
{
    char fullPath[STRING_ARRAY_SIZE];
    if (getcwd(fullPath, sizeof(fullPath)) != NULL) {
        strcat(fullPath, "/");
        strcat(fullPath, (char*) path);
        strcat(fullPath, (char*) fileName);
    } else {
        perror("getcwd error");
        exit(1);
    }

    if (remove(fullPath) == 0) {
        printf("File %s deleted successfully.\n", fullPath);
    } else {
        perror("Error deleting the file");
    }
}


int main()
{
    // update meta date of each directory
    update_meta();


    // Implementing help


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
        commands_count--;  

        // create a process to do tasks
        char ptype[STRING_ARRAY_SIZE];
        strcpy(ptype, commands[0]);

        for (int task = 1 ; task <= commands_count ; task++)
        {
            char order[4][STRING_ARRAY_SIZE];
            const char space_delimiter[] = " ";
            char* stripped_task = strip(commands[task]);
            char* Stoken = strtok(stripped_task, space_delimiter);

            char temp_buffer[STRING_ARRAY_SIZE];
            int j = 0;
            while (Stoken != NULL && j < STRING_ARRAY_SIZE) {
            char* stripped_task = strip(Stoken);
            strcpy(temp_buffer, stripped_task);
            strcpy(order[j], temp_buffer);
            Stoken = strtok(NULL, space_delimiter);
            j++;
            }

            if (strcmp(order[0], "create") == 0){
                create_file(order[1], order[2], order[3]);
            }
            else if (strcmp(order[0], "read") == 0){
                read_file(order[1], order[2]);
            }
            else if (strcmp(order[0], "update") == 0){
                update_file(order[1], order[2], order[3]);
            }
            else if (strcmp(order[0], "delete") == 0){
                delete_file(order[1], order[2]);
            }
            else{
                perror("Wrong Inputes");
                exit(1);
            }

        }




    }

    return 0;
}