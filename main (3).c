#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void swap(int *a,int *b){
	int x;
	x=*a;
	*a=*b;
	*b=x;  		

	
	
	
}





int main(int argc, char *argv[]) {
	int m,n;
	
	scanf("%d,%d",&m,&n);
	if(m<1||m>9999||n<1||n>9999)
	{
	printf("error");
    }
	else
	{
	void swap(int *a,int *b); 
	if(m>n)
	{
	swap(&m,&n);
    }
	int i,j;
	int k=0;
	int s=0;
    int sum=0;
for(i=m;i<=n;i++)
{
  
	for(j=1;j<i;j++)
	{
	if(i%j==0)
	  {sum=sum+j;
	  }
    }
	if(i==sum)	
	{ k++;}	
	sum=0;	
		
}
	for(i=m;i<=n;i++)
{
  
	for(j=1;j<i;j++)
	{
	if(i%j==0)
	  {sum=sum+j;
	  }
    }
	if(i==sum)	
{ s++;
	printf("%d",i);
	if(s<k)
	{
	printf(",");}
}	
	sum=0;	
		
}
}
	
	
	
	return 0;
}



