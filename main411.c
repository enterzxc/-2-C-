#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int judge(int a)
{
	int i;
	for (i = 2; i < a; i++)
	{
		if (a % i == 0)
			return 0;
    }  
	        return 1; 
}




int main(int argc, char *argv[]) {
	int m, n;
	scanf("%d,%d", &m, &n);
	int i;
	for (i = m; i <= n-2; i++)
	{
		if(judge(i)&&judge(i+2))
			printf("%d,%d\n",i,i+2);
	}


	
	
	
	
	
	return 0;
}
