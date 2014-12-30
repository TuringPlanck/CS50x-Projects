/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BLOCK 512

int main(int argc, char* argv[])
{
   // open input file
   FILE* inptr = fopen("card.raw", "r");
   
   // ensure proper usage
   if (inptr == NULL)
    {
        printf("Could not open card.raw \n");
        return 2;
    }
    
    // declare 512 byte buffer to store JPEG blocks
    uint8_t buffer[BLOCK];
    
    int file_count = 0;
    FILE* outptr = NULL;
    
    // iterate over infile's scanlines
    while (fread(buffer, BLOCK, 1, inptr))
    {
        // check for JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
        (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
            // close outfile if open
            if (outptr != NULL)
            {
                fclose(outptr);
            }
            
            // format title
            char title[8];
            sprintf(title, "%03d.jpg", file_count);
            
            // open new JPEG for writing
            outptr = fopen(title, "w");
            
            file_count++;
        }
        
        // write to current JPEG file
        if (outptr != NULL)
        {
            fwrite(buffer, BLOCK, 1, outptr);
        }
    }
    
    // close outfile if open
    if (outptr != NULL)
    {
        fclose(outptr);
    }
    
    // close infile
    fclose(inptr);
    
    return 0;     
}
