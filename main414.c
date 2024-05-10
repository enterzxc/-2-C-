#include<stdio.h>
float project(int n, int x)
{
	if (n == 0)
		return 1;
	if (n == 1)
		return x;

	return ((2 * n - 1) * x - project(n - 1, x) - (n - 1) * project(n - 2, x))/n;
}
int main()
{
	int n, x;
	scanf("%d,%d", &n, &x);
	if (n < 0) {
		printf("error");	
	}
	else {
		printf("%.2f", project(n, x));
	}
	return 0;	
}
