#include<stdio.h>
int main()
{
	int b,c,d,e;
	int a=0;//赋值表达式 ，a的值为0 
	printf("a=%d\n",a);
	b=(a+3)*5;//算数表达式，b的值为 15
	printf("b=%d\n",b) ;
	c=a&&b;//逻辑表达式 ，c的值为 0
	printf("c=%d\n",c); 
	d=a<b;//关系表达式 ，d的值为 1
	printf("d=%d\n",d);
	//下面为与常识不同的表达式的值 
	e=9/2;//除法，应该得到4.5，但结果为4 
	printf("e=%d\n",e);
	return 0; 
	
}
