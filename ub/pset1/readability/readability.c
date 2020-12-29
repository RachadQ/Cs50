#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
void ColemanLiau(float l, float s);
float AverageLtoW();
float AverageStoW();

int letters = 0;
int words = 0;
int sentences = 0;
int main(void)
{
    //prompt user for text
    string text = get_string("Text: %c ", isprint(13));
    // loop through the text 
  
    for (int i = 0; i < strlen(text); i++)
    {
      
    
        //check if not a space
        if (!isspace(text[i]))
        {
            //if not punctation either then increase the letter
            if (!ispunct(text[i]))
            {
                letters++;
            }
            else
            {
                //if next letter
                if (text[i] == '.' || text[i] == '?' || text[i] == '!')
                {
                    sentences++;
                }
            }
        }
        //is a space
        else
        { 
            //check if space is followed by a letter
            if (isalpha(text[i + 1]) || !ispunct(text[i]))
            {
               
                words++;
          
            }
        
        }
        
        if (i == strlen(text) - 1)
        {
            words++;
        }
    }
    ColemanLiau(AverageLtoW(), AverageStoW());
 

}

void ColemanLiau(float l, float s)
{
    float letterCount = 0.0588 * l;
    float wordCount = 0.296 * s;
    float coleman = (letterCount - wordCount) - 15.8;
    int index = round(coleman);
    //check if level under 1
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    //check if index is higher then 16
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
   
}
 
float AverageLtoW()
{
    float average;
    //cast int to float
    float l = (float)letters;
    float w = (float)words;
    //get average letter to word
    average = (l / w) * 100;
     
    return average;
    
}

float AverageStoW()
{
    float average;
    //cast int to float
    float s = (float)sentences;
    float w = (float)words;
    //get average letter to word
    average = (s / w) * 100;
    return average;
}

