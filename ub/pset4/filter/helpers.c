#include "helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>  
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double average;
    //loop through 2d array
    for (int h = 0; h < height; h++)
    {

        for (int w = 0; w < width; w++)
        {
            //get Average
            average = SetAverage(image[h][w].rgbtRed, image[h][w].rgbtGreen, image[h][w].rgbtBlue);
            //set average
            image[h][w].rgbtRed = average;
            image[h][w].rgbtGreen = average;
            image[h][w].rgbtBlue = average;

        }
    }

}   

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    double sepiaRed;
    double sepiaGreen;
    double sepiaBlue;

    //loop through 2d array
    for (int h = 0; h < height; h++)
    {

        for (int w = 0; w < width; w++)
        {

            float ogRed =  image[h][w].rgbtRed  ;
            float ogGreen = image[h][w].rgbtGreen;
            float ogBlue = image[h][w].rgbtBlue;
        
            sepiaRed = (.393 * ogRed) + (.769 * ogGreen) + (.189 * ogBlue) ;
            sepiaGreen = (.349 * ogRed)  + (.686 * ogGreen) + (.168 * ogBlue);
            sepiaBlue = (.272 * ogRed) + (.534 * ogGreen) + (.131 * ogBlue);
            // round sepia colours
            sepiaRed = round(sepiaRed);
            sepiaGreen = round(sepiaGreen);
            sepiaBlue = round(sepiaBlue);
            // check if they pass 255 limit if they do set to 255
            if (sepiaRed > 255)
            {
                image[h][w].rgbtRed = 255;
            }
            else
            {
                image[h][w].rgbtRed = (int)sepiaRed;
            }
            if (sepiaGreen > 255)
            {
                image[h][w].rgbtGreen = 255;
            }
            else
            {
                image[h][w].rgbtGreen = (int)sepiaGreen;
            }
            if (sepiaBlue > 255)
            {
                image[h][w].rgbtBlue = 255;
            }
            else
            {
                image[h][w].rgbtBlue = (int)sepiaBlue;
            }
        }
    }


    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //get max width
    int maxWidth = width;
    double dWidth = width;
    //get the middle value and round it
    double middle = round(dWidth / 2);
    
    RGBTRIPLE temp;
    //check remaiinder is 0
    if (width % 2 == 0)
    {

        for (int h = 0; h < height; h++)
        {
            for (int w = 0; w < (int)middle; w++)
            {
                //if not in the middle of the image
                if (w != (int)middle)
                {
                    //assign Attempt
                    temp = image[h][w];
                    image[h][w] = image[h][maxWidth - 1 - w];
                    image[h][width - 1 - w] = temp;

                }   
            }
            maxWidth = width;
        }
    }
    else
    {
        for (int h = 0; h < height; h++)
        {
            for (int w = 0; w < (int)middle; w++)
            {

                temp = image[h][w];
                image[h][w] = image[h][maxWidth - 1 - w];
                image[h][width - 1 - w] = temp;

            }
            maxWidth = width;
        }

    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE normal[height][width];
    //iterate through the copy
    for (int h  = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //make a copy 
            normal[h][w] = image[h][w];
        }
    }
    
    for (int h = 0, counter; h < height; h++)
    {
        double red;
        double green;
        double blue;
        for (int w = 0; w < width; w++)
        {
            //set red blue and green to zero
            red = green = blue = counter = 0;
            //if height and width is greater then 0
            if (h >= 0 && w >= 0)
            {
                red += normal[h][w].rgbtRed;
                green += normal[h][w].rgbtGreen;
                blue += normal[h][w].rgbtBlue;
                counter++;
            }
            //if height and inbound left
            if (h >= 0 && w - 1 >= 0)
            {
                red += normal[h][w - 1].rgbtRed;
                green += normal[h][w - 1].rgbtGreen;
                blue += normal[h][w - 1].rgbtBlue;
                counter++;
            }
            //if height greater then 0 and w + 1 greater or  = to zero 
            if ((h >= 0 && w + 1 >= 0) && (h >= 0 && w + 1 < width))
            {
                red += normal[h][w + 1].rgbtRed;
                green += normal[`h][w + 1].rgbtGreen;
                blue += normal[h][w + 1].rgbtBlue;
                counter++;
            }
            if (h - 1 >= 0 && w >= 0)
            {
                red += normal[h - 1][w].rgbtRed;
                green += normal[h - 1][w].rgbtGreen;
                blue += normal[h - 1][w].rgbtBlue;
                counter++;
            }
            if (h - 1 >= 0 && w - 1 >= 0)
            {
                red += normal[h - 1][w - 1].rgbtRed;
                green += normal[h - 1][w - 1].rgbtGreen;
                blue += normal[h - 1][w - 1].rgbtBlue;
                counter++;
            }
            if ((h - 1 >= 0 && w + 1 >= 0) && (h - 1 >= 0 && w + 1 < width))
            {
                red += normal[h - 1][w + 1].rgbtRed;
                green += normal[h - 1][w + 1].rgbtGreen;
                blue += normal[h - 1][w + 1].rgbtBlue;
                counter++;
            }
            if ((h + 1 >= 0 && w >= 0) && (h + 1 < height && w >= 0))
            {
                red += normal[h + 1][w].rgbtRed;
                green += normal[h + 1][w].rgbtGreen;
                blue += normal[h + 1][w].rgbtBlue;
                counter++;
            }
            if ((h + 1 >= 0 && w - 1 >= 0) && (h + 1 < height && w - 1 >= 0))
            {
                red += normal[h + 1][w - 1].rgbtRed;
                green += normal[h + 1][w - 1].rgbtGreen;
                blue += normal[h + 1][w - 1].rgbtBlue;
                counter++;
            }
            if ((h + 1 >= 0 && w + 1 >= 0) && (h + 1 < height && w + 1 < width))
            {
                red += normal[h + 1][w + 1].rgbtRed;
                green += normal[h + 1][w + 1].rgbtGreen;
                blue += normal[h + 1][w + 1].rgbtBlue;
                counter++;
            }
            //divide counters by how much  around it
            double roundRed, roundGreen, roundBlue;
            roundRed = round(red / counter);
            roundGreen = round(green / counter);
            roundBlue = round(blue / counter);
            image[h][w].rgbtRed = (int)roundRed;
            image[h][w].rgbtGreen = (int)roundGreen;
            image[h][w].rgbtBlue = (int)roundBlue;
        }
    }
    
}

int SetAverage(int red, int green, int blue)
{
    //add colours together
    double add = (red + green + blue) ;
    //get average and   round it
    double average = round(add / 3)  ;
    //casting average
    int converted = (int)average;
    return converted;
}


