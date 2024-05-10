#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int  judge(int num)
{
	int a[100]={0}, b[100] = {0},count,j=0,i,flag=0;
	for (count = 0;num!=0; count++)
	{a[count] = num % 10;
		num = num / 10;
		
	}
	count--;
	for (i = count; i >= 0; i--,j++)
		b[i] = a[j];

	for (i = 0; i <= count; i++)
	{
		if (a[i] != b[i])
		{
			flag = 1;
			break;
		}
	}
	if (flag)
		return 0;
	return 1;

}
int main(int argc, char *argv[]) {
	
int n,i;
int count = 0;
	scanf("%d", &n);
	for (i = 0; i <= n; i++)
	{
		if (judge(i))
		   { count++;
			printf("%d,", i);
			if(count%10==0)
			printf("\n");
		   }   
	}
	return 0;
}

