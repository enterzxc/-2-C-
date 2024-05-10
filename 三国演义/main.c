#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
int main()
{
	FILE* pf1 = fopen("三国演义.txt", "r");

	int count = 0;
	char num;

	while (count <= 21150)
	{
		num = fgetc(pf1);
		printf("%c", num);
		count++;
	}
	fclose(pf1);
	pf1 = NULL;
	pf1 = fopen("三国演义.txt", "r");
	FILE* pf2 = fopen("新三国演义.txt", "w");
	while (num != -1)
	{
		num = fgetc(pf1);
		fputc(num, pf2);
	}
	fclose(pf1);
	fclose(pf2);
	return 0;
}