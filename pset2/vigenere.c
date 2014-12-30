#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>

int main (int argc, string argv[])
{
    //Checks for single command line argument
    if (argc != 2)
    {
        printf("Please provide a single command line argument for k \n");
        return 1;
    }
    
    //Checks that argument is alphabetical
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Your keyword must consist of alphabetic characters only. Please try again. \n");
            return 1;
        }
    }
    
    //Sets variables, gets plaintext from user
    int q = strlen(argv[1]);
    string key = argv[1];
    string plaintext = GetString();
    int n = strlen(key);
    
    //Sets key characters to alphabetical index between 0 and 25
    for (int i = 0; i < n; i++)
    {
        if (isupper(key[i]))
        {
            key[i] = key[i] - 65;
        }
        else if (islower(key[i]))
        {
            key[i] = key[i] - 97;
        }
    }
    
    //Modifies plaintext by each letter in key
    //Prints encrypted characters on same line
    //Returns key index to 0 when wrapping around.
    //Note: could have used modulo operator to acheve same effect
    for (int i = 0, j = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
            {
                printf("%c", ((plaintext[i] - 65 + key[j]) % 26 + 65));
            }
            else if (islower(plaintext[i]))
            {
                printf("%c", ((plaintext[i] - 97 + key[j]) % 26 + 97));
            }
            j++;
            if (j == q)
            {
                j = 0;
            }
        }
        
        //Skips non-alphabetical characters 
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
    return 0;
}
