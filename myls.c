// #include <stdio.h>
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
// #include <fcntl.h>

bool option_i = false;
bool option_R = false;
bool option_l = false;

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

    // printf("path: %s %s\n", path, dir);
    struct node *ptr = malloc(sizeof(struct node));
    // char *copyDir = malloc(sizeof(strlen(dir)));

    int size = strlen(path) + strlen(dir) + strlen("\"\"");

    char *filePath = malloc(size);

    strcpy(filePath, path);
    strcat(filePath, "/");
    // strcat(filePath, "/\"");

    strcat(filePath, dir);
    // strcat(filePath, "\"");
    // printf("%s\n", filePath);
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

    // char *directory = malloc(sizeof(strlen(ptr->directory)));
    // strcpy(directory, ptr->directory);

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

    // printf("%s\n", filePath);

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

void swap(int i, int j, char **array)
{
    char *temp = array[i];

    array[i] = array[j];
    array[j] = temp;
}

int w1_less_w2(char *word1, char *word2)
{
    int minlength = strlen(word1) < strlen(word2) ? strlen(word1) : strlen(word2);

    char *copy1 = malloc(strlen(word1));
    char *copy2 = malloc(strlen(word2));
    strcpy(copy1, word1);
    strcpy(copy2, word2);
    for (int i = 0; i <= minlength; i++)
    {
        copy1[i] = tolower(copy1[i]);
        copy2[i] = tolower(copy2[i]);
    }

    bool b = false;
    if (strcmp(copy1, copy2) < 0)
    {

        b = true;
    }

    return b;
}

void printArray(char **names, int numberOfName)
{
    // printf("Name array:\n");
    for (int i = 0; i < numberOfName; i++)
    {
        printf("%s\n", names[i]);
    }
}

int partition(char **array, int left, int right)
{
    char *pivot = array[right];
    int index = left - 1;

    for (int i = left; i < right; i++)
    {
        if (w1_less_w2(array[i], pivot))
        {
            index++;
            swap(i, index, array);
        }
    }
    swap(index + 1, right, array);
    return index + 1;
}

void sortLexographically(char **names, int left, int right)
{

    if (left < right)
    {
        int pivot = partition(names, left, right);
        sortLexographically(names, left, pivot - 1);
        sortLexographically(names, pivot + 1, right);
    }
}

void sort(char **names, int numberOfNames)
{

    sortLexographically(names, 0, numberOfNames - 1);
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

    if (S_ISDIR(sb.st_mode))
    {

        return true;
    }

    return false;
}
void recursiveShow(char *path)
{

    DIR *folder;
    struct dirent *entry;
    printf("Path is: %s\n", path);
    folder = opendir(path);

    if (folder)
    {
        int entryLength = 10;
        char **entryNames = malloc(sizeof(char *) * entryLength);
        int numberOfEntries = 0;

        while (entry = readdir(folder))
        {
            entryNames[numberOfEntries] = entry->d_name;

            numberOfEntries++;
            if (numberOfEntries > entryLength)
            {
                entryLength += 10;
                entryNames = realloc(entryNames, sizeof(char *) * entryLength);
            }
        }

        sort(entryNames, numberOfEntries);
        printArray(entryNames, numberOfEntries);
        for (int i = 0; i < numberOfEntries; i++)
        {
            if (isDirectory(entryNames[i], path))
            {
                // printf("enqueuing: %s \n", entryNames[i]);
                enqueue(entryNames[i], path);
            }
        }

        // displayQ();
        free(entryNames);
    }
    else
    {
        printf("Empty\n");
    }
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
        printf("\n");

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
    else{
        printf("Error: %s not found\n", path);
    }
}

void showDir(char *path)
{
    DIR *folder;
    struct dirent *entry;
    // printf("Path is: %s\n", path);

    folder = opendir(path);

    if (folder)
    {
        int entryLength = 10;
        char **entryNames = malloc(sizeof(char *) * entryLength);
        int numberOfEntries = 0;

        while (entry = readdir(folder))
        {
            entryNames[numberOfEntries] = entry->d_name;
            numberOfEntries++;
            if (numberOfEntries > entryLength)
            {
                entryLength += 10;

                entryNames = realloc(entryNames, sizeof(char *) * entryLength);
            }
        }

        sort(entryNames, numberOfEntries);
        printArray(entryNames, numberOfEntries);
        free(entryNames);
    }
    else
    {
        printf("Empty\n");
    }
}

int main(int argc, char const *argv[])
{
    // option_i = false;
    // option_R = false;
    // option_l = false;
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
                }else{
                    printf("Error: %c flag is invalid\n", argv[i][j]);

                }

            }
        }
        if ( argv[i][0] != '-')
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
    

    // recursiveShow(PATH);
    if (option_R)
    {
        struct node *ptr;

        ptr = first;
        while (first)
        {
            ptr = dequeue();

            // printf("%s\n", ptr->path);
            printDirectory(ptr->path);
            free(ptr->path);
            free(ptr);
            // displayQ();
        }
    }

    return 0;
}
