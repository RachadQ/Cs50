#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void SwitchLetters(int addBy, string word);
int EncryptUpper(int letter, int position);
int EncryptLower(int letter, int position);

int main(int argc, string argv[])
{
    // check if argc is  greater then 2
    if (argc > 2 || argc == 1)
    {
        
        printf("usage ./caesar key");
        return 1;
    }
    else if (argc == 2)
    {
        for (int i = 0; i < strlen(argv[1]); i++)
        {
                        if (isalpha(argv[1][i]))
            {
                printf("usage ./caesar key");
                return 1;
              
            }
          
        
        }
        
        //convert string to into int
        int convert = atoi(argv[1]);
        printf("%i\n", convert);
        //prompt user for word to cipher
        string cipher = get_string("cipher ");
       
        //loop through each letter of cipher
        SwitchLetters(convert, cipher);
    }
    
  
  
}

void SwitchLetters(int addBy, string word)
{
     
    //loop through each letter
    for (int i = 0; i < strlen(word); i++)
    {
        if (isalpha(word[i]))
        {
              
            if (isupper(word[i]))
            {
                
              
                //check if pass 90
                int check = word[i] + addBy;
                
                if (check > 90)
                {
                    check = word[i];
                    word[i] = EncryptUpper(check, addBy);
                   
                    
                    
                }
                else
                {
                    
                    word[i] = check;
                    
                }
               
                //Loop through ascii capital
            }
            else if (islower(word[i]))
            {
                //check if pass 122
                int check = word[i] + addBy;
               
                if (check > 122)
                {
                    check = word[i];
                    word[i] = EncryptLower(check, addBy);
                     
                    
                }
                else
                {
                    word[i] = check;
                    
                }
                
                
               
                //loop through ascii lower case
            }
            
        }
      
        
        
       
        
    
    }
    printf("ciphertext: %s\n", word);
}
//upper
int EncryptUpper(int letter, int position)
{
    int c = ((letter + position - 65) % 26 + 65);
   
    return c;
}
//  lower
int EncryptLower(int letter, int position)
{
    int c = ((letter + position - 97) % 26 + 97);
    return c;
}

