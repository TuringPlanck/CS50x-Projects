/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
int findMid(int imin, int imax)
{
    return imin + (imax - imin)/2;
}
bool binarySearch(int value, int values[], int imin, int imax)
{
    //Checks for empty set
    if (imax < imin)
    return false;
    
    //Finds midpoint of array
    int imid = findMid(imin, imax);
    
    //3 way comparison
    if (values[imid] < value)
        //Search upper set
        return binarySearch(value, values, imid + 1, imax);
    
    else if (values[imid] > value)
        //Search lower set
        return binarySearch(value, values, imin, imid-1);    
    else
        //Key found
        return true;
}  
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int imin = 0;
    int imax = n;
    return binarySearch(value, values, imin, imax);
}
/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    for (int i = 0; i < n; i++)
    {
        int min = values[i];
        int index;
        for (int j = i + 1; j <= n; j++)
        {
            if (values[j] < min)
            {
                min = values[j];
                index = j;
            }
        }
        if (min != values[i])
        {
            values[index] = values[i];
            values[i] = min;  
        }
    }
}
