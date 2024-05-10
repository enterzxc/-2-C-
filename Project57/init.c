#include<stdio.h>
#include<stdlib.h>

int main()
{
	char arr[1000], ch;
	int flag = 0, i = 0, q = 0, pcount = 0;
	char num[3] = { 0 };
	int j = 0, pnum = 0, number = 0;
	while ((ch = getchar()) != '\n')
	{
		arr[i] = ch;
		i++;
	}
	for (i = 0; arr[i] != ';'; i++)
	{
		if (arr[i] == '[')
		{
			num[0] = arr[i + 1];
			if (arr[i + 2] != ']' && arr[i + 2] != ' ')
			{
				num[1] = arr[i + 2];
				if (arr[i + 3] != ']' && arr[i + 2] != ' ')
					num[2] = arr[i + 3];
			}

		}
		if (arr[i] == '{')
		{
			flag = 1;
			if (arr[i + 2] != '}')
			{
				q = i;
			}
			else
				q = -1;

		}

	}

	for (j = 0; j < 3; j++)
	{
		if (num[j] == ' ')
			num[j] = 0;
		if (num[j] == 0)
		{
			number++;
		}

	}

	if (number == 0)
		pnum = (num[0] - 48) * 100 + (num[1] - 48) * 10 + num[2] - 48;
	if (number == 1)
		pnum = (num[0] - 48) * 10 + num[1] - 48;
	if (number == 2)
		pnum = num[0] - 48;
	if (flag == 0)
	{
		for (i = 0; i < pnum; i++)
			printf("N ");
	}

	if (flag == 1 && q != -1)
	{
		for (i = q + 2; arr[i] != '}'; i++)
		{
			if (arr[i] != ',' && arr[i] != ' ')
			{
				printf("%c", arr[i]);
				if (arr[i + 1] != ' ' && arr[i + 1] != ',') {
					i++;
					while (arr[i] != ' ' && arr[i] != ',' && arr[i] != '}') {

						printf("%c", arr[i]);
						i++;
					}


				}
				printf(" ");
				pcount++;
			}

		}
		for (i = 0; i < pnum - pcount; i++)
		{
			printf("0 ");
		}
	}
	if (q == -1 && flag == 1)
		for (i = 0; i < pnum; i++)
		{
			printf("0 ");
		}
	return 0;
}