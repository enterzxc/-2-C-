#include <stdio.h>
#include <stdlib.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	int a,b,c,d,e,f,g,h;
	scanf("%d",&a);

	b=a*a*a*a;
	c=(a+3)*(a+3)*(a+3)*(a+3);
	d=(a+6)*(a+6)*(a+6)*(a+6);
	e=(a+9)*(a+9)*(a+9)*(a+9);
	f=(a+12)*(a+12)*(a+12)*(a+12);
	g=(a+15)*(a+15)*(a+15)*(a+15);
	h=b+c+d+e+f+g;
	
	if(a>=3 && a<=10)
	{printf("%d",h);
	}
	else
	{printf("error");
	}
	return 0;
}
