#include<stdio.h>
int main()
{
	int b,c,d,e;
	int a=0;//��ֵ���ʽ ��a��ֵΪ0 
	printf("a=%d\n",a);
	b=(a+3)*5;//�������ʽ��b��ֵΪ 15
	printf("b=%d\n",b) ;
	c=a&&b;//�߼����ʽ ��c��ֵΪ 0
	printf("c=%d\n",c); 
	d=a<b;//��ϵ���ʽ ��d��ֵΪ 1
	printf("d=%d\n",d);
	//����Ϊ�볣ʶ��ͬ�ı��ʽ��ֵ 
	e=9/2;//������Ӧ�õõ�4.5�������Ϊ4 
	printf("e=%d\n",e);
	return 0; 
	
}
