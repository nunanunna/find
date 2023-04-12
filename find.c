#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

void PrintDirData(struct dirent *entry);

int main(int argc, char *argv[])
{

    char *cwd = (char *)malloc(sizeof(char) * 1024);

    if (argc == 1) {
        getcwd(cwd, 1024);
    } else {
        strcpy(cwd, argv[1]);
    }

    DIR *dir = NULL;
    struct dirent *entry = NULL;

    dir = opendir(cwd);

    while ((entry = readdir(dir)) != NULL)
    {
        PrintDirData(entry);
    }

    printf("\n");

    free(cwd);

    closedir(dir);

    return 0;
}

void PrintDirData(struct dirent *entry)
{
    printf("%s\n", entry->d_name);
}
