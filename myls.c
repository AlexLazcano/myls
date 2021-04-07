#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
// #include <fcntl.h>

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
    // char *copyDir = malloc(sizeof(strlen(dir)));

    char *filePath = malloc(sizeof(strlen(path) + strlen(dir) + strlen("/\"") + 1));

    // strcpy(copyDir, dir);
    // strcpy(copyPath, path);
    strcpy(filePath, path);
    strcat(filePath, "/\"");
    strcat(filePath, dir);
    strcat(filePath, "\"");

    ptr->path = filePath;
    // ptr->path = copyPath;
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

void printLastModified(char *entryName, char *path)
{
    char filePath[100];
    struct stat sb;
    struct tm *date;
    char tmbuf[64], buf[64];

    strcpy(filePath, path);
    strcat(filePath, "/");
    strcat(filePath, entryName);

    // printf("%s\n", filePath);

    stat(filePath, &sb);
    time_t time = (time_t)sb.st_mtim.tv_sec;
    date = localtime(&time);

    strftime(tmbuf, sizeof tmbuf, "%b %d %Y %H:%M", date);

    printf("%s %s\n", tmbuf, entryName);
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
    if (strcmp(entry, ".git") == 0)
    {
        return false;
    }

    char filePath[100];
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
        for (int i = 0; i < numberOfEntries; i++)
        {
            if (isDirectory(entryNames[i], path))
            {
                enqueue(entryNames[i], path);
            }
            // printf("%s\n", entryNames[i]);
        }
        printArray(entryNames, numberOfEntries);
        displayQ();
        struct node *ptr;

        if (first)
        {
            ptr = dequeue();
            printf("%s\n", ptr->path);
            recursiveShow(ptr->path);
        }
    }
    else
    {
        printf("Empty\n");
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
    // showDir(PATH);

    recursiveShow(PATH);

    return 0;
}
