#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	int a,b,c,d,e,f;
	
	scanf("%d,%d",&a,&b);
	
	c=a+b;
    d=a-b;
    e=a*b;
    f=a/b;
    if(b==0)
    printf("a+b=%d\na-b=%d\na*b=%d\na/b=Œﬁ“‚“Â",c,d,e);
    else
   printf("a+b=%d\na-b=%d\na*b=%d\na/b=%d",c,d,e,f);
	return 0;
}
