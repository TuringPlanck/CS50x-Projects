#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>

int main (int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Please provide a single command line argument for k \n");
        return 1;
    }
    int k = atoi(argv[1]);
    if (k < 0)
    {
        printf("Please provide a non-negative integer for k \n");
        return 2;
    }
    else
    {
        string plaintext = GetString();       
        for (int i = 0, n = strlen(plaintext); i < n; i++)
        {
            if (isalpha(plaintext[i]))
            {
                if (isupper(plaintext[i]))
                {
                printf("%c", ((plaintext[i] - 65 + k) % 26 + 65));
                }
                if (islower(plaintext[i]))
                {
                printf("%c", ((plaintext[i] - 97 + k) % 26 + 97));
                }
            }
            else
            {
            printf("%c", plaintext[i]);   
            }
        }
    }
printf("\n");
return 0;
}   
