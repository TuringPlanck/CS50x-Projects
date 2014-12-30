#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float change;
    printf("O hai!");
    
    //get change in dollars
    do
    {
        printf("How much change is owed?\n");
        change = GetFloat();
    }
    while (change <=0);
   
   //convert dollars to cents
   change = round(change*100);
   int c = change;
   
   //determine how many of each coin is needed (using largest coins first when possible)
   int q = 0;
   int d = 0;
   int n = 0;
   int p = 0;
   
   if (c >= 25)
   {
       q = c/25 - (c%25)/c;
   }
   if ((c%25) >= 10)
   {
       d = (c%25)/10 - ((c%25)%10)/(c%25);
   }
   if (((c%25)%10) >= 5)  
   { 
       n = ((c%25)%10)/5 - (((c%25)%10)%5)/((c%25)%10);
   }
   if ((((c%25)%10)%5) >= 1)
   {
       p = (((c%25)%10)%5)/1 - ((((c%25)%10)%5)%1)/(((c%25)%10)%5);
   }
  
   //sum coins
   int count = q + d + n + p;
   printf("%d\n", count);
}
