#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>
 
int main(int argc, char* argv[])
{
		
        char * cwd = (char *)malloc(sizeof(char) * 1024);
        
        DIR * dir = NULL;
        struct dirent * entry = NULL;
 
        while((entry = readdir(dir)) != NULL)
        {
                printf("%s\n", entry->d_name);
        }
 
        printf("\n");
        
        free(cwd);
 
        closedir(dir);
 
        return 0;
 
}

void PrintDirData(struct dirent* entry) {
    printf("%s\t", entry->d_ino);
}