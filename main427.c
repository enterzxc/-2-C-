#include<stdio.h>
void main()
{
 int x;
 for (x=1;x<100;x++)
 if (++x%2==0)
 if (++x%3==0)
 if (++x%5==0)
 printf("%d ",x);
}
