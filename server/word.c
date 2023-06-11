#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char* word()
{
    char letters[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    srand(time(0));
    int arrlength = sizeof(letters) / sizeof(letters[0]);
    int rWordlength = rand() % 26;
    char *word = malloc(32);
    
    for(int i = 0; i < rWordlength ; i++)
    {
        int rLetter = rand() % arrlength;
        word[i] = letters[rLetter];
    }

    return word;
}