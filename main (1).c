#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
float project(int n,int x)
{
	if(n==0)
	{return 1;
	}
	else if(n==1)
	{return x;
	}
	else 
	{return ((2*n-1)*x-project(n-1,x)-(n-1)*project(n-2,x))/n;
	}
}
int main(int argc, char *argv[]) {
	int n,x;
	scanf("%d,%d",&n,&x);
	if(n<0)
	{
		printf("error");
	}
	else
	{
		printf("%.2f",project(n,x));
	}

	return 0;
}

