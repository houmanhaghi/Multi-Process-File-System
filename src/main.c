#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void create_sub_dir(char* sub_dir_name)
{
    char directory_path[100] = "./root/";
    strcat(directory_path, sub_dir_name);
    printf("%s\n", directory_path);
    int result = mkdir(directory_path, 0766);
    if (result == 0) {
        printf("Directory created successfully.\n");
    } else {
        perror("Error creating directory\n");
        exit(1);
    }
}


int main()
{

    // need admin access to create folders

    // Implementing root directory and other sub-directories in here
    const char* root_path = "./root";
    int result = mkdir(root_path, 0766);
    if (result == 0) {
        printf("Root directory created successfully.\n");
    } else {
        perror("Error creating root directory\n");
        exit(1);
    }

    create_sub_dir("bin");



    return 0;
}