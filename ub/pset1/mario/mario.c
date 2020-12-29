#include <stdio.h>
#include <cs50.h>

void MakeBoard(int);

int main(void)
{
    int getHeight;
    //Prompt user for input
     do
     {
         getHeight = get_int("how much rows?");

     }
     while( getHeight <= 0  || getHeight > 8);

    MakeBoard(getHeight);
    return 0;
}
//create Board
 void MakeBoard(int amount)
{
    //make left side
    //get Rows
    for( int row = 0; row < amount; row++)
    {
       //create spaces
       for(int dots = amount-1; dots > row;  dots--)
        {
           printf(" ");
        }


    //createcolumn
      for(int column = 1; column <= row ; column++)
        {
              printf("#");
        }
        printf("#\n");
    }



}
