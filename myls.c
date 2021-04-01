#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char const *argv[])
{
    bool option_i = false;
    bool option_R = false;
    bool option_l = false;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (int j = 1; j < strlen(argv[i]); j++)
            {
                printf("%c\n", argv[i][j]);

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
    }

   


    return 0;
}
