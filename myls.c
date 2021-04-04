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

    // printf("word 1: %s\n", word1);
    // printf("word 2: %s\n", word2);
    for (int i = 0; i < minlength; i++)
    {
        if (word1[i] < word2[i])
        {
            // printf("%s Goes first\n", word2);
            return true;
        }
    }
    return false;
}

// int w1_more_w2(char *word1, char *word2)
// {

//     int minlength = strlen(word1) < strlen(word2) ? strlen(word1) : strlen(word2);

//     // printf("word 1: %s\n", word1);
//     // printf("word 2: %s\n", word2);
//     for (int i = 0; i < minlength; i++)
//     {
//         if (word1[i] > word2[i])
//         {
//             // printf("%s Goes first\n", word2);
//             return true;
//         }
//     }
//     return false;
// }

void printArray(char **names, int numberOfName)
{
    printf("Name array:\n");
    for (int i = 0; i < numberOfName; i++)
    {
        printf("%d %s\n", i, names[i]);
    }
}

int partition(char **array, int left, int right)
{
    int pivot = right;
    int index = left - 1;

    // printf("pivot = %s\n", array[pivot]);
    for (int i = left; i < right; i++)
    {
        if (w1_less_w2(array[i], array[pivot]))
        {
            index++;
            swap(index, i, array);
        }
    }

    swap(index + 1, right, array);
    return index + 1;
}

void sortLexographically(char **names, int left, int right)
{

    if (left < right)
    {

        int q = partition(names, left, right);
        // printf("partition %d\n", q);
        sortLexographically(names, left, q - 1);
        sortLexographically(names, q + 1, right);
    }
}

void sort(char **names, int numberOfNames)
{

    printArray(names, numberOfNames);

    sortLexographically(names, 0, numberOfNames - 1);
    printArray(names, numberOfNames);
}

void showDir(char *path)
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

    showDir(PATH);

    return 0;
}
