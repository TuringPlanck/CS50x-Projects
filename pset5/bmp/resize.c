/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Resizes a BMP by a factor of n.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy n infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    
    // get n
    int n = atoi(argv[1]);
    if (n <= 0 || n > 100) 
    {
        printf("Usage: ./copy n infile outfile\n");
        return 1;
    } 

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // saves orignal image height & width
    int initWidth = bi.biWidth;
    int initHeight = bi.biHeight;
    
    // resizes image height & width
    bi.biWidth = bi.biWidth * n;
    bi.biHeight = bi.biHeight * n;
    
    // determine padding for scanlines
    int newPadding = (4 - (bi.biWidth * sizeof (RGBTRIPLE)) % 4) % 4;
    int initPadding = (4 - (initWidth * sizeof (RGBTRIPLE)) % 4) % 4;
    
    // compute new image size
    bi.biSizeImage = abs(bi.biHeight) * ((bi.biWidth * sizeof (RGBTRIPLE)) + newPadding);
    
    // compute new file size
    bf.bfSize = bi.biSizeImage + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(initHeight); i < biHeight; i++)
    {
        for (int w = 0; w < n; w++)
        {                        
            // iterate over pixels in scanline
            for (int j = 0; j < initWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write n x RGB triple to outfile
                for (int z = 0; z < n; z++)
                {
                   fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr); 
                }
            }

            // add padding
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }
            
            // return to beginning of scanline
            if (w < n - 1)
            {
                fseek(inptr, -initWidth * sizeof(RGBTRIPLE), SEEK_CUR);
            }
        }
        
        // skip over padding, if any
        fseek(inptr, initPadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
