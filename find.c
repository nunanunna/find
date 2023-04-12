#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

enum{
    MAX_DIRECTORY_LENGTH = 1024
};

void PrintDirData(char* file_name, struct stat *stat_data);

int main(int argc, char *argv[])
{
    char *cwd = (char *)malloc(sizeof(char) * MAX_DIRECTORY_LENGTH);
    char *file_name = (char *)malloc(sizeof(char) * MAX_DIRECTORY_LENGTH);;
    getcwd(cwd, MAX_DIRECTORY_LENGTH);

    char backslash[] = "\\";

    struct stat file_status;
    DIR *file_dir = NULL;
    struct dirent *file = NULL;

    file_dir = opendir(cwd);

    if (argc != 1) {
        strcpy(file_name, argv[1]);
        strcat(cwd, backslash);
        strcat(cwd, file_name);
        stat(cwd, &file_status);
        PrintDirData(file_name, &file_status);
    }

    while ((file = readdir(file_dir)) != NULL)
    {
        stat(cwd, &file_status);
        PrintDirData(file->d_name, &file_status);
    }

    printf("\n");

    free(cwd);

    closedir(file_dir);
    system("PAUSE");
    return 0;
}

void PrintDirData(char* file_name, struct stat *stat_data)
{
    struct tm* t = localtime(&stat_data->st_ctime);

    printf("%d-%d-%d\t", t->tm_year, t->tm_mon, t->tm_mday);
    if(t->tm_hour >= 12)
        printf("오후 %d:%d\t", t->tm_hour, t->tm_min);
    else
        printf("오전 %d:%d\t", t->tm_hour, t->tm_min);
    printf("%s\n", file_name);
}
