#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


// #ifdef _WIN32
// #include <direct.h>
// #define mkdir(path, mode) _mkdir(path)
// #endif


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
        .dir_path = "root/",
        .total_size = 100000000,
        .free_size = 0,
        .number_of_files = 8,
        .permission = {
            .read = {"t0", "t1", "t2", "t3", "t5"},
            .write = {"t0", "t1", "t2", "t3", "t4"}
        }
    },
    {
        .dir_path = "root/bin/",
        .total_size = 50000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3", "t5"},
            .write = {"t0", "t1", "t2", "t3"}
        }
    },
    {
        .dir_path = "root/dev/",
        .total_size = 20000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3", "t5"},
            .write = {"t0", "t4"}
        }
    },
    {
        .dir_path = "root/etc/",
        .total_size = 2000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3", "t5"},
            .write = {"t0", "t1", "t2"}
        }
    },
    {
        .dir_path = "root/home/",
        .total_size = 100000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3", "t5"},
            .write = {"t0", "t1", "t2", "t3"}
        }
    },
    {
        .dir_path = "root/mnt/",
        .total_size = 3000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3"},
            .write = {"t0", "t2", "t3"}
        }
    },
    {
        .dir_path = "root/proc/",
        .total_size = 5000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t5"},
            .write = {"t0", "t1", "t4"}
        }
    },
    {
        .dir_path = "root/tmp/",
        .total_size = 25000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1", "t2", "t3"},
            .write = {"t0", "t1"}
        }
    },
    {
        .dir_path = "root/usr/",
        .total_size = 40000000,
        .free_size = 0,
        .number_of_files = 0,
        .permission = {
            .read = {"t0", "t1"},
            .write = {"t0"}
        }
    }
};


char** splitString(const char* input, int* numTokens) {
    char** tokens = NULL;
    char* token;
    char* copy = strdup(input);
    int count = 0;

    // Count the number of tokens (substrings) in the input string
    token = strtok(copy, "/");
    while (token) {
        count++;
        token = strtok(NULL, "/");
    }

    // Allocate memory for the tokens array
    tokens = (char**)malloc(count * sizeof(char*));

    // Split the input string and store the tokens in the array
    int i = 0;
    token = strtok((char*)input, "/");
    while (token) {
        tokens[i] = strdup(token);
        i++;
        token = strtok(NULL, "/");
    }

    // Update the numTokens variable with the number of tokens
    *numTokens = count;

    return tokens;
}


int have_permission(char* ptype, char* path, int read , int write)
{
    // printf("%s\n", path);
    if (write == 1){
        // bool flag = false;
        for (int i = 0 ; i < len(meta) ; i++){
            
            if (strcmp(meta[i].dir_path, path) == 0){
                // flag = true;
                for (int j = 0 ; j < len(meta[i].permission.write) ; j++){
                    if (strcmp(ptype, meta[i].permission.write[j]) == 0)
                        return 1;
                }
            }
        }
        // if (!flag){
        //         int numTokens;
        //         int last_in_path;
                
        //         char** tokens = splitString(path, &numTokens);
        //         for (int j = 0 ; j < len(meta) ; j++){
        //             char** last_path = splitString(meta[j].dir_path, &last_in_path);
        //             for (int i = numTokens-1 ; i >= 0 ; i-- ){
        //                 // printf("== %s :: %s == ||", last_path[last_in_path-1], tokens[i]);
        //                 if (strcmp(last_path[last_in_path-1],tokens[i]) == 0)
        //                     return 1;
        //             }
        //         }

        //         // Free allocated memory
        //         for (int i = 0; i < numTokens; i++) {
        //             free(tokens[i]);
        //         }
        //         free(tokens);
        // }
        return 0;
    }


    else if (read == 1){
        // bool flag = false;
        for (int i = 0 ; i < len(meta) ; i++){
            if (strcmp(meta[i].dir_path, path) == 0){
                for (int j = 0 ; j < len(meta[i].permission.read) ; j++){
                    if (strcmp(ptype, meta[i].permission.read[j]) == 0)
                        return 1;
                }
            }
        }
        // if (!flag){
        //         int numTokens;
        //         int last_in_path;
                
        //         char** tokens = splitString(path, &numTokens);
        //         for (int j = 0 ; j < len(meta) ; j++){
        //             char** last_path = splitString(meta[j].dir_path, &last_in_path);
        //             for (int i = numTokens-1 ; i >= 0 ; i-- ){
        //                 // printf("== %s :: %s == ||", last_path[last_in_path-1], tokens[i]);
        //                 if (strcmp(last_path[last_in_path-1],tokens[i]) == 0)
        //                     return 1;
        //             }
        //         }

        //         // Free allocated memory
        //         for (int i = 0; i < numTokens; i++) {
        //             free(tokens[i]);
        //         }
        //         free(tokens);
        // }
        return 0;
    }


    return 0;
}


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
            printf("stat error\n");
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


int makeDirectory(const char *path) {
    #if defined(_WIN32)
    // For Windows, use _mkdir() function
    return _mkdir(path);
    #else
    // For Unix-like systems, use mkdir() function
    return mkdir(path, 0777); // 0777 gives read, write, and execute permissions to all users
    #endif
}


void update_meta()
{
    char path[STRING_ARRAY_SIZE]; 
    for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
            path[x] = '\0';
    if (getcwd(path, sizeof(path)) != NULL) {
    } else {
        printf("getcwd error: %s\n", path);
    }

    int index = 0;
    while(strcmp(meta[index].dir_path, "\0") != 0)
    {
        FILE *file;
        char directory_path[STRING_ARRAY_SIZE];
        for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
            directory_path[x] = '\0';
        strcat(directory_path, path);
        strcat(directory_path ,"/");
        strcat(directory_path, meta[index].dir_path);
        char meta_path[STRING_ARRAY_SIZE];
        for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
            meta_path[x] = '\0';
        strcat(meta_path ,directory_path);
        strcat(meta_path, ".meta");


        file = fopen(meta_path, "w");
        
        // Check if the file was opened successfully
        if (file == NULL) {
        printf("Error opening the file: %s\n", meta_path);
        }

        long int current_size = getFolderSize(directory_path);
        if (current_size == -1) {
            printf("Error: Unable to get the folder size.\n");
        }


        // Print directory metadata
        fprintf(file, "Directory: %s\n", directory_path);

        // fprintf(file, "Total size: %lu bytes\n", vfs_stat.f_frsize * vfs_stat.f_blocks);

        fprintf(file, "Total size: %lu bytes\n", meta[index].total_size);

        fprintf(file, "Free space: %lu bytes\n", meta[index].total_size - current_size );
        meta[index].free_size = meta[index].total_size - current_size;

        if (meta[index].total_size - current_size < 0){
            printf("Storage Warnning! Storage of directory: %s  is out of range", directory_path);
        }

        fprintf(file, "Number of files: %d\n", countFilesRecursively(directory_path)-1);
        meta[index].number_of_files = countFilesRecursively(directory_path)-1;

        fclose(file);
        index++;
    }
    
}



// implement function to do CRUD tasks
void create_file(char* path, char* fileName, char* write_string )
{

    FILE *file;
    char fullPath[100*STRING_ARRAY_SIZE];
    
    if (getcwd(fullPath, sizeof(fullPath)) != NULL) {
        strcat(fullPath, "/");
        strcat(fullPath, (char*) path);
        if (makeDirectory(fullPath) == 0)
            {
                
            }
        strcat(fullPath, (char*) fileName);
    } else {
        printf("getcwd error: %s\n", fullPath);
    }
    
    file = fopen(fullPath, "w");
    if (file == NULL) {
        printf("Error opening the file: %s\n", fullPath);
    }

    fprintf(file, "%s\n", (char *)write_string);

    fclose(file);
    
    printf("File %s created successfully\n", fullPath);
    update_meta();

}


void read_file(char* path, char* fileName)
{
    FILE *file;
    char fullPath[STRING_ARRAY_SIZE];
    if (getcwd(fullPath, sizeof(fullPath)) != NULL) {
        strcat(fullPath, "/");
        strcat(fullPath, (char*) path);
        // strcat(fullPath, "/");
        strcat(fullPath, (char*) fileName);
    } else {
        printf("getcwd error\n");
    }
    // printf("%s\n", fullPath);
    file = fopen(fullPath, "r");
    if (file == NULL) {
        printf("Error opening the files: %s\n", fullPath);
    }

    // printf("%s\n", fullPath);

    printf("File %s read successfully: \n", fullPath);
    char temp_line[STRING_ARRAY_SIZE];
    while (fgets(temp_line, sizeof(temp_line), file) != NULL) {
        printf("%s", temp_line);
    }

    fclose(file);
    
}


void update_file(char* path, char* fileName, char* append_string )
{

    // append file in here
    FILE *file;
    char fullPath[STRING_ARRAY_SIZE];
    if (getcwd(fullPath, sizeof(fullPath)) != NULL) {
        strcat(fullPath, "/");
        strcat(fullPath, (char*) path);
        strcat(fullPath, (char*) fileName);
    } else {
        printf("getcwd error\n");
    }

    file = fopen(fullPath, "a");
    if (file == NULL) {
        printf("Error opening the file: %s\n", fullPath);
    }

    fprintf(file, "%s\n", (char *)append_string);

    fclose(file);

    printf("File %s updated successfully: \n", fullPath);
    
    update_meta();

}


void delete_file(char* path, char* fileName)
{
    char fullPath[STRING_ARRAY_SIZE];
    if (getcwd(fullPath, sizeof(fullPath)) != NULL) {
        strcat(fullPath, "/");
        strcat(fullPath, (char*) path);
        strcat(fullPath, (char*) fileName);
    } else {
        printf("getcwd error\n");
    }

    // printf("%s", fullPath);

    if (remove(fullPath) == 0) {
        printf("File %s deleted successfully: \n", fullPath);
    } else {
        perror("Error deleting the file");
    }
    update_meta();
}


int main(int argc, char* argv[])
{
    // update meta date of each directory
    update_meta();


    // Implementing help
    if (argc == 2 && (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") ==0)){
        printf("FiSys: multi process file handling\n");
        printf("Usage: ./FiSys\n");
        printf("to handle this program, you should write your commands in configs.txt.\n");
        printf("each line introduce a separate type process, that declared in first argument.\n");
        printf("after that, commands run concurently. we have 4 type of commands:\n\n");
        printf("CRUD\n");
        printf("C: create path fileName [write_string]\n");
        printf("R: read   path fileName\n");
        printf("U: update path fileName [append_string]\n");
        printf("D: delete path fileName\n\n");
        printf("each time you run the program, all directories will be updated.\n");
        printf("whole of the changes will be occure in the root directory that has implemented in the project directory.");

        printf("\n");
        exit(0);
    }


    // read commands from configs.txt
    FILE *file;
    char line[MAX_LINE_LENGTH][STRING_ARRAY_SIZE];
    for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
                for (int y = 0 ; y < STRING_ARRAY_SIZE ; y++)
                 line[y][x] = '\0';

    char config_path[STRING_ARRAY_SIZE]; 
    for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
            config_path[x] = '\0';
    if (getcwd(config_path, sizeof(config_path)) != NULL) {
        strcat(config_path, "/src/");
        strcat(config_path, CONFIG);
    } else {
        printf("getcwd error: %s\n", config_path);
        
    }

    file = fopen(config_path, "r");
    if (file == NULL) {
        printf("Error opening the file: %s\n", config_path);
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
        char commands[STRING_ARRAY_SIZE][STRING_ARRAY_SIZE];
        for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
                for (int y = 0 ; y < STRING_ARRAY_SIZE ; y++)
                 commands[y][x] = '\0';

        const char delimiter[] = ",";

        // Strip leading and trailing whitespace from the line
        char* stripped_line = strip(line[p]);

        // Tokenize the stripped_line and store tokens in commands array
        char* token = strtok(stripped_line, delimiter);
        int commands_count = 0; // Initialize to 0 since no tokens are found yet
        char token_buffer[STRING_ARRAY_SIZE];
        for (int y = 0 ; y < STRING_ARRAY_SIZE ; y++)
                 token_buffer[y]= '\0';

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
        for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
            ptype[x] = '\0';
        strcpy(ptype, commands[0]);

        for (int task = 1 ; task <= commands_count ; task++)
        {
            char order[32][STRING_ARRAY_SIZE];
            for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
                for (int y = 0 ; y < 32 ; y++)
                 order[y][x] = '\0';
            const char space_delimiter[] = " ";
            char* stripped_task = strip(commands[task]);
            char* Stoken = strtok(stripped_task, space_delimiter);

            char temp_buffer[STRING_ARRAY_SIZE];
            for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
                 temp_buffer[x] = '\0';
            int j = 0;
            while (Stoken != NULL && j < STRING_ARRAY_SIZE) {
            char* stripped_task = strip(Stoken);
            strcpy(temp_buffer, stripped_task);
            strcpy(order[j], temp_buffer);
            Stoken = strtok(NULL, space_delimiter);
            j++;
            }

            if (strcmp(order[0], "create") == 0){
                char nt[STRING_ARRAY_SIZE];
                strcpy(nt, order[1]) ;
                if (true){    //if (have_permission(ptype, order[1], 0, 1)){
                    strcpy(order[1], nt);
                    char to_send[STRING_ARRAY_SIZE];
                    for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
                        to_send[x] = '\0';

                    int counter = 3;
                    while (strcmp(order[counter], "\0") != 0 ){
                        strcat(to_send, order[counter]);
                        strcat(to_send, " ");
                        counter++;
                    }
                    if (to_send[0] == '\0')
                        strcpy(to_send, "");
                    create_file(order[1], order[2], to_send);
                }else{
                    printf("Access Denied: %s %s %s\n", order[0], order[1], order[2]);
                }
            }
            else if (strcmp(order[0], "read") == 0 ){
                char nt[STRING_ARRAY_SIZE];
                strcpy(nt, order[1]) ;
                if (have_permission(ptype, order[1], 1, 0)){
                    strcpy(order[1], nt);
                    read_file(order[1], order[2]);
                }else{
                    printf("Access Denied: %s %s %s\n", order[0], order[1], order[2]);
                }
            }
            else if (strcmp(order[0], "update") == 0 ){
                char nt[STRING_ARRAY_SIZE];
                strcpy(nt, order[1]) ;
                if (have_permission(ptype, order[1], 0, 1)){
                    strcpy(order[1], nt);
                    char to_send[STRING_ARRAY_SIZE];
                    for (int x = 0 ; x < STRING_ARRAY_SIZE ; x++)
                        to_send[x] = '\0';

                    int counter = 3;
                    while (strcmp(order[counter], "\0") != 0 ){
                        strcat(to_send, order[counter]);
                        strcat(to_send, " ");
                        counter++;
                    }
                    if (to_send[0] == '\0')
                        strcpy(to_send, "");
                    update_file(order[1], order[2], to_send);
                }else{
                    printf("Access Denied: %s %s %s\n", order[0], order[1], order[2]);
                }
            }
            else if (strcmp(order[0], "delete") == 0){
                char nt[STRING_ARRAY_SIZE];
                strcpy(nt, order[1]) ;
                if (have_permission(ptype, order[1], 0, 1)){
                    strcpy(order[1], nt);
                    delete_file(order[1], order[2]);
                }else{
                    printf("Access Denied: %s %s %s\n", order[0], order[1], order[2]);
                }
            }
            else{
                printf("Wrong Inputes: %s %s %s\n", order[0], order[1], order[2]);
            }

        }

    }

    return 0;
}
