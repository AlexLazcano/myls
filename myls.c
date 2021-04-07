
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include "secret_headers.h"

bool option_i = false;
bool option_R = false;
bool option_l = false;
char* name = "";

struct node
{
    char *path;
    // char *directory;
    struct node *next;
};

struct node *first = NULL;
struct node *last = NULL;

void enqueue(char *dir, char *path)
{
    struct node *ptr = malloc(sizeof(struct node));

    int size = strlen(path) + strlen(dir) + strlen("\"\"");

    char *filePath = malloc(size);

    strcpy(filePath, path);
    strcat(filePath, "/");

    strcat(filePath, dir);

    ptr->path = filePath;

    ptr->next = NULL;
    if (!first)
    {
        first = ptr;
        last = ptr;
    }
    else
    {
        last->next = ptr;
        last = ptr;
    }
}

struct node *dequeue()
{
    struct node *ptr;
    ptr = first;
    first = ptr->next;

    return ptr;
}
void displayQ()
{
    struct node *ptr;

    ptr = first;
    printf("Queue: \n");
    while (ptr)
    {
        printf("%s\n", ptr->path);
        ptr = ptr->next;
    }
}

char *getPermissions(mode_t bits)
{
    int n = 10;
    // char *permisions = malloc(n*sizeof(char));
    char permisions[] = "rwxrwxrwx";

    //user
    if ((bits & S_IRWXU) == 0) // read write exec
    {
        permisions[0] = '-';
        permisions[1] = '-';
        permisions[2] = '-';
    }
    else
    {
        if ((bits & S_IRUSR) == 0) // read
        {
            permisions[0] = '-';
        }

        if ((bits & S_IWUSR) == 0) // write
        {

            permisions[1] = '-';
        }
        if ((bits & S_IXUSR) == 0) // exec
        {
            permisions[2] = '-';
        }
    }

    //group
    if ((bits & S_IRWXG) == 0) // read write exec
    {
        permisions[3] = '-';
        permisions[4] = '-';
        permisions[5] = '-';
    }
    else
    {
        if ((bits & S_IRGRP) == 0) // read
        {
            permisions[3] = '-';
        }

        if ((bits & S_IWGRP) == 0) // write
        {
            permisions[4] = '-';
        }
        if ((bits & S_IXGRP) == 0) // exec
        {
            permisions[5] = '-';
        }
    }
    //other
    if ((bits & S_IRWXO) == 0) // read write exec
    {
        permisions[6] = '-';
        permisions[7] = '-';
        permisions[8] = '-';
    }
    else
    {
        if ((bits & S_IROTH) == 0) // read
        {
            permisions[6] = '-';
        }

        if ((bits & S_IWOTH) == 0) // write
        {
            permisions[7] = '-';
        }
        if ((bits & S_IXOTH) == 0) // exec
        {
            permisions[8] = '-';
        }
    }

    char *ptr = malloc(strlen(permisions) * sizeof(char));
    strcpy(ptr, permisions);
    return ptr;
}

void printFile(char *entryName, char *path)
{
    char filePath[100];
    struct stat sb;
    struct tm *date;
    char tmbuf[64], buf[64];

    struct group *grp;
    struct passwd *user;

    strcpy(filePath, path);
    strcat(filePath, "/");
    strcat(filePath, entryName);

    stat(filePath, &sb);
    time_t time = (time_t)sb.st_mtim.tv_sec;
    date = localtime(&time);
    char *permisions;

    int n = strftime(tmbuf, sizeof tmbuf, "%b %d %Y %H:%M", date);

    permisions = getPermissions(sb.st_mode);

    if (option_i)
    {
        printf("%-9ld ", sb.st_ino);
    }

    if (option_l)
    {
        printf("%-10s %ld %s %s %6ld %-18s ", permisions, sb.st_nlink, getpwuid(sb.st_uid)->pw_name, getgrgid(sb.st_gid)->gr_name, sb.st_size, tmbuf);
    }

    printf("%s\n", entryName);

    free(permisions);
}

void printArray(char **names, int numberOfName)
{
    // printf("Name array:\n");
    for (int i = 0; i < numberOfName; i++)
    {
        printf("%s\n", names[i]);
    }
}

bool isDirectory(char *entry, char *path)
{
    if (strcmp(entry, ".") == 0)
    {
        return false;
    }
    if (strcmp(entry, "..") == 0)
    {
        return false;
    }

    char filePath[500];
    struct stat sb;
    struct tm *date;
    char tmbuf[64], buf[64];

    strcpy(filePath, path);
    strcat(filePath, "/");
    strcat(filePath, entry);
    stat(filePath, &sb);
    if (S_ISLNK(sb.st_mode))
    {

        return true;
    }

    if (S_ISDIR(sb.st_mode))
    {

        return true;
    }

    return false;
}

int fileSearch(const struct dirent *entry){
    return !strcmp(entry->d_name, name );
}
void printDirectory(char *path)
{
    struct dirent **files;

    int numberOfFiles = scandir(path, &files, NULL, alphasort);
    if (numberOfFiles >= 0)
    {
        for (int i = 2; i < numberOfFiles; i++)
        {
            printFile(files[i]->d_name, path);
        }
        printf("-------\n");

        for (int i = 0; i < numberOfFiles; i++)
        {
            if (isDirectory(files[i]->d_name, path))
            {
                // printf("enqueuing: %s \n", entryNames[i]);
                enqueue(files[i]->d_name, path);
            }
        }
        free(files);
    }
    else
    {
        // int index = 0;
        char *ptr;
        ptr = strrchr(path, '/');
        char copyPath[strlen(path)];
        char file[strlen(path)];

        int index = 0;

        for (int i = strlen(path); i > 0; i--)
        {
            if (path[i] == '/')
            {

                index = i + 1;
                break;
            }
        }

        strncpy(copyPath, path, index);
        strncpy(file, path + index, strlen(path) - index);
        file[strlen(path) - index] = '\0';


        name = file;
        struct dirent **files;

        int numberOfFiles = scandir(copyPath, &files, fileSearch, NULL);

        if (numberOfFiles > 0)
        {
           printf("%s\n", path);
        }else{
            printf("Error: %s not found\n", path);
        }
        
        name = "";
        

        
    }
}

int main(int argc, char const *argv[])
{

    char PATH[100];

    strcpy(PATH, ".");

    int numberOfPaths = 0;

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
                else
                {
                    printf("Error: %c flag is invalid\n", argv[i][j]);
                }
            }
        }
        if (argv[i][0] != '-')
        {
            numberOfPaths++;
            // strcpy(PATH, argv[i]);
        }
    }

    // printDirectory(PATH);
    for (int i = argc - numberOfPaths; i < argc; i++)
    {

        strcpy(PATH, argv[i]);

        printDirectory(PATH);
    }
    if (numberOfPaths == 0)
    {
        printDirectory(".");
    }

    // recursiveShow(PATH);
    if (option_R)
    {
        struct node *ptr;

        ptr = first;
        while (first)
        {
            ptr = dequeue();

            printf("%-20s\n", ptr->path);
            printDirectory(ptr->path);
            free(ptr->path);
            free(ptr);
            // displayQ();
        }
    }

    return 0;
}
