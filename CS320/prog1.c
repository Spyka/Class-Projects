/********************************************************************************
Class:         CS320 
Assignment:    Programming Assignment 1
Date Due:    3/14/13
********************************************************************************/

#include <stdio.h>

/*Prototypes*/

void promptFraction(char *buffer);
void printFinalNumber(int n, int d);
void reduce(int *n, int *d, int gcd);
void invalid(void);
int validate_input(const char *s, int *slash_position);
int num_den_val(char *num, char *den, int *numerator, int *denominator);
int is_negative(int numerator, int denominator);
int GCD(int numerator, int denominator);

int main(void)
{
    char buffer[30];
    int slash_position;
    int numerator;
    int denominator;    
    int gcd; 
    int valOne;
    int valTwo;
    
    printf("Jacqueline Behr, masc0859 \n");
    
    for(;;)
    {
        slash_position = NULL;    /*Reset SlashPosition*/
        
        promptFraction(buffer);    /*Ask for a fraction*/
        
    
        /*check if only digits, negatives, slash*/
        valOne = validate_input(buffer,&slash_position);
    
        /*If fraction isn't just digits or doesn't have a slash print
        invalid statement and restart*/
        if(valOne == 0 || slash_position == NULL)
        {
            invalid();
            rewind(stdin);
            continue;    
        }
    
        
        /*Splitting the String to get Num and Den seperate*/
        char num[30] = {'\0'};        /*array initialized to null*/
        strncpy(num, buffer, slash_position);
        char den[30] = {'\0'};
        strcpy(den, &(buffer[slash_position+1]));
    
        /*make sure the numerator and denominator are within parameters*/
        valTwo = num_den_val(num, den, &numerator, &denominator);
    
        /*If not valid print invalid statement and restart*/
        if(valTwo == 0)
        {
            invalid();
            rewind(stdin);
            continue;
        }
    
        /*check the negativity/positivity of the fraction*/
        is_negative(numerator,denominator);
    
        /*find greatest common denominator for reducing*/
        gcd = GCD(numerator, denominator);
    
        reduce(&numerator, &denominator, gcd);
    
        printFinalNumber(numerator, denominator);
    
        /*loop back if user wants to go again, otherwise terminate
        program*/
        while(1)
        {
            rewind(stdin);
            printf("Would you like to reduce another fraction (y/n)? \n");
            scanf("%1s", buffer);
        
            if(!(*buffer == 'y' || *buffer == 'n'))
            {
                invalid();
            }
        
            if(*buffer == 'y' || *buffer == 'n')
                break;
        }
    
        if(*buffer == 'y')
        {
            rewind(stdin);
            continue;
        }
    
        if(*buffer == 'n')
            break;
    }
    
    return 0;
    
    
}

void promptFraction(char *buffer)
{
    printf("Please enter a fraction to reduce: ");
    scanf("%25s", buffer);
}

int validate_input(const char *s, int *slash_position)
{
    if(strlen(s) > 25)    /*is the string longer than 25 chars?*/
        return 0;    /*false*/
    int i = s[0] == '-' ? 1 : 0;    /*is there a negative*/
    for(; i<strlen(s); i++)        /*loop to get digit positions and slash
                    positions, check to see if second neg 
                    in denom*/
        {
            if(isdigit(s[i]));
            else if(s[i] == '/')
                *slash_position = i;
            else if(s[i] == '-' ? 1 :0);
            else
                return 0;
        }
    return 1;
}

int num_den_val(char *num, char *den, int *numerator, int *denominator)
{
    const int MAX_INT = 0x7FFFFFFF;
    const int MIN_INT = 0x80000000;
    
    /*Make sure num and denom are within parameters, if so convert to int*/
    long long toss = atoll(num);
    if(toss > MAX_INT || toss < MIN_INT)
        {
            return 0;    /*not valid - false*/
        }    
    *numerator = atoi(num);
    
    toss = atoll(den);
    if(toss > MAX_INT || toss < MIN_INT)
        {
            return 0;
        }
    *denominator = atoi(den);
    
    if(*denominator == 0)
        {
            return 0;
        }
}

int is_negative(int numerator, int denominator)
{
    return (numerator^denominator) < 0 ? 1 : 0;
}

int GCD(int numerator, int denominator)
{
    if (denominator==0)
        return numerator;
    else
        return GCD(denominator, numerator%denominator);
}

void reduce(int *n, int *d, int gcd)
{
    *n = *n/gcd;
    *d = *d/gcd;
}

void printFinalNumber(int n, int d)
{
    printf("The reduced faction is: %d/%d \n", n, d);
}

void invalid(void)
{
    printf("Sorry, invalid input. \n");
    rewind(stdin);
}
