#include<stdio.h>
int main()
{
	int a[1000] = { 0 }, b[1000] = { 0 };
	int num, max = -1, i, q,j;
	scanf("%d", &num);
	for (i = 0; i < num; i++)
		scanf("%d", &a[i]);
	if (num != 1)
	{
		for (i = 0; i < num - 1; i++)
			for (j = i + 1; j < num; j++)
			{
				if (a[j] < a[i])
				{
					q = a[j];
					a[j] = a[i];
					a[i] = q;
				}
			}
		for (i = 0; i < num - 1; i++)
			for (j = i + 1; j < num; j++)
			{
				if (a[j] == a[i])
					b[i]++;
				if (b[i] > max)
					max = b[i];
			}
		for (i = 0; i < num; i++)
		{
			if (b[i] == max)
				printf("%d %d\n", a[i], b[i] + 1);
		}
		return 0;

	}
	else
		printf("%d 1", a[0]);
	return 0;
	
}
