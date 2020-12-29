#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <unistd.h>
void Greedy(int);




int main(void)
{
    int cents;
    float change;
    //Prompt user for input
    do
    {
        change = get_float("Change Owed: ");

    }
    while (change < 0);
    //turn dollars to cents
    cents = round(change * 100);
    //conver to cahnge
    printf("%d\n", cents);
    Greedy(cents);
  
}
//get the highest change
void Greedy(int amount)
{
    int count = 0;
    do
    {
        //if greater then 25
        if (amount >= 25)
        {
           
            //check if money amount is 0
            if (amount > 0)
            {
                amount -= 25;
                //increase count
                count += 1 ;
               
            }
            
        }
        //is greater then 10 but less then 25
        else if (amount >= 10 && amount < 25)
        {
            //check if money amount is 0
            if (amount > 0)
            {
                amount -= 10;
                count += 1;
               
            }
        }
        //if amount is greater the 5cent but less then 25 cent and 10 cents
        else if (amount >= 5 && amount < 25 && amount < 10)
        {
            //check if money amount is 0
            if (amount > 0)
            {
                amount -= 5;
                //increase count
                count += 1; 
                
            }
        }
        //if greater then 1 cent but less then 25 10 and 5
        else if (amount >= 1  && amount < 25 && amount < 10 && amount < 5)
        {
            //check if money amount is 0
            if (amount > 0)
            {
                amount -= 1;
                //increase count
                count += 1;
               
            }
        }
    }
    while (amount > 0);
    
    printf("Total change is:%d\n", count);
}    





