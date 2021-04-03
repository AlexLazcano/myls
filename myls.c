#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
// #include <fcntl.h>

void printLastModified(struct dirent *entry, char* path)
{
    char filePath[100];
    struct stat sb;
    struct tm *date;
    char tmbuf[64], buf[64];

    strcpy(filePath, path);
    strcat(filePath, "/");
    strcat(filePath, entry->d_name);

    // printf("%s\n", filePath);

    stat(filePath, &sb);
    time_t time = (time_t)sb.st_mtim.tv_sec;
    date = localtime(&time);

    strftime(tmbuf, sizeof tmbuf, "%b %d %Y %H:%M", date);

    printf("%s %s\n", tmbuf, entry->d_name);
}

void showls(char *path)
{
    DIR *folder;
    struct dirent *entry;
    printf("Path is: %s\n", path);
    folder = opendir(".");
    
    
    if (folder)
    {
        while (entry = readdir(folder))
        {
            printLastModified(entry, path);
            // printf("%s\n", entry->d_name);
            // printf("%ld\n", entry->d_off);
        }
    }
    else
    {
        printf("Empty\n");
    }
}

int main(int argc, char const *argv[])
{
    bool option_i = false;
    bool option_R = false;
    bool option_l = false;
    char PATH[100];
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (int j = 1; j < strlen(argv[i]); j++)
            {
                // printf("%c\n", argv[i][j]);

                if (argv[i][j] == 'i')
                {
                    option_i = true;
                }
                else if (argv[i][j] == 'R')
                {
                    option_R = true;
                }
                else if (argv[i][j] == 'l')
                {
                    option_l = true;
                }
            }
        }
        if (i == argc - 1 && argv[i][0] != '-')
        {
            strcpy(PATH, argv[i]);
        }
    }

    showls(".");

    return 0;
}
