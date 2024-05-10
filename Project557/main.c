#include<stdio.h>
int main()
{
	int n, count = 0;
	scanf_s("%d", &n);
	float a, b, c, sum = 0;
	for (a = 2.0, b = 1.0; count < n; count++)
	{
		sum = a / b + sum;
		c = a;
		a = c + b;
		b = c;
	}
	printf("%.2f", sum);
	return 0;


}