#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


uint8_t buffer[512];
uint8_t checkBuff[4];
int jpgCount = 0;
FILE *img;
int main(int argc, char *argv[])
{   
    
    // Check for invalid usage
    if (argc != 2)
    {   
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    //open jpeg File
    FILE *fp;
    //read memory card file
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("couldn't read %s", argv[1]);
    }

    char picha[8];
   
    while (fread(buffer, 512, 1, fp))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (jpgCount > 0)
            {
                //close the already open file
                fclose(img);

                //name outfile using sprintf
                sprintf(picha, "%03d.jpg", jpgCount);
                jpgCount ++;

                //open newfile for writing
                img = fopen(picha, "w");

                // Write to outfile
                fwrite(buffer, sizeof(buffer), 1, img);
            }
            if (jpgCount == 0)
            {
                //name outfile using sprintf
                sprintf(picha, "%03d.jpg", jpgCount);
                jpgCount++;

                //open newfile for writing
                img = fopen(picha, "w");

                // Write to outfile
                fwrite(buffer, sizeof(buffer), 1, img);
            }



        }
        else if (jpgCount > 0)
        {

            fwrite(buffer, sizeof(buffer), 1, img);
        }

    
            
    }
    fclose(fp);
    fclose(img);
    //close MemoryCard
    
    return 0;
    
   
}
