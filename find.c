#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <windows.h>
#include <locale.h>

enum{
    MAX_DIRECTORY_LENGTH = 1024,
    MAX_SERIAL_NUM_LENGTH = 9
};

typedef struct VolumeInfo {
    char volume_name[MAX_PATH + 1];
    char volume_serial[MAX_SERIAL_NUM_LENGTH];
} VolInfo;

void GetVolumeInfo(VolInfo* volume_info, char* cwd);
void PrintDirData(char* file_name, struct stat *stat_data);
void PrintVolumeInfo(VolInfo* volume_info, char* cwd);
unsigned long long int GetVolumeSize(char* cwd);


int main(int argc, char *argv[])
{
    setlocale(LC_NUMERIC, "");

    char *cwd = (char *)malloc(sizeof(char) * MAX_DIRECTORY_LENGTH);
    char *file_name = (char *)malloc(sizeof(char) * MAX_DIRECTORY_LENGTH);;
    getcwd(cwd, MAX_DIRECTORY_LENGTH);

    char backslash[] = "\\";

    struct stat file_status;
    DIR *file_dir = NULL;
    struct dirent *file = NULL;

    VolInfo volume_info;

    int dir_count = 0;
    int file_count = 0;
    long total_file_size = 0;

    GetVolumeInfo(&volume_info, cwd);
    PrintVolumeInfo(&volume_info, cwd);

    file_dir = opendir(cwd);

    if (argc != 1) {
        strcpy(file_name, argv[1]);
        strcat(cwd, backslash);
        strcat(cwd, file_name);
        stat(cwd, &file_status);
        PrintDirData(file_name, &file_status);
        return 0;
    }
    
    while ((file = readdir(file_dir)) != NULL)
    {
        strcat(cwd, backslash);
        strcat(cwd, file->d_name);
        stat(cwd, &file_status);
        PrintDirData(file->d_name, &file_status);
        getcwd(cwd, MAX_DIRECTORY_LENGTH);

        if(S_ISDIR(file_status.st_mode))
            dir_count++;
        if(S_ISREG(file_status.st_mode))
            file_count++;

        total_file_size += file_status.st_size;
    }

    printf("%'16d개 파일%'20d 바이트\n", file_count, total_file_size);
    printf("%'16d개 디렉터리%'17llu 바이트 남음\n", dir_count, GetVolumeSize(cwd));

    free(cwd);
    free(file_name);

    closedir(file_dir);
    return 0;
}

void PrintDirData(char* file_name, struct stat *stat_data)
{
    // 날짜/시간 출력
    struct tm* t = localtime(&stat_data->st_atime);
    

    printf("%d-%02d-%02d  ", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
    if(t->tm_hour >= 12)
        printf("오후 %02d:%02d    ", t->tm_hour-12, t->tm_min);
    else if(t->tm_hour == 12)
        printf("오후 %02d:%02d    ", 12, t->tm_min);
    else
        printf("오전 %02d:%02d    ", t->tm_hour, t->tm_min);
    
    // 디렉터리 여부 출력
    if(S_ISDIR(stat_data->st_mode))
        printf("<DIR>          ");
    else
        printf("%'14d ", stat_data->st_size);

    printf("%s\n", file_name);
}

void GetVolumeInfo(VolInfo* volume_info, char* cwd) {
    
    char rootPath[] = {cwd[0], cwd[1], cwd[2]}; 
    DWORD vol_serial; 
    DWORD max_comp_len; 
    DWORD fs_flags; 
    char fs_name[MAX_PATH + 1]; 

    BOOL result = GetVolumeInformationA(
        rootPath,
        volume_info->volume_name, 
        sizeof(volume_info->volume_name),
        &vol_serial, 
        &max_comp_len,
        &fs_flags, 
        fs_name,
        sizeof(fs_name));

    sprintf(volume_info->volume_serial, "%08X", vol_serial);
}

void PrintVolumeInfo(VolInfo* volume_info, char* cwd) {
    if(volume_info->volume_name == NULL)
        printf(" %c 드라이브의 볼륨에는 이름이 없습니다.\n", cwd[0]);
    else
        printf(" %c 드라이브의 볼륨: %s\n", cwd[0], volume_info->volume_name);
    
    printf(" 볼륨 일련 번호: %c%c%c%c-%c%c%c%c\n\n", 
    volume_info->volume_serial[0], volume_info->volume_serial[1], volume_info->volume_serial[2], volume_info->volume_serial[3],
    volume_info->volume_serial[4], volume_info->volume_serial[5], volume_info->volume_serial[6], volume_info->volume_serial[7]);

    printf(" %s 디렉터리\n\n", cwd);
}

unsigned long long int GetVolumeSize(char* cwd) {
    ULARGE_INTEGER total, free;
    GetDiskFreeSpaceEx("C:\\", NULL, &total, &free);// C드라이브의 정보를 얻음

    return free.QuadPart;
}