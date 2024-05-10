#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	char a, b,c,d,e,af,bf,cf,df,ef;
	scanf("%c%c%c%c%c",&a,&b,&c,&d,&e);
    if(a>=119)
    af=(a-26+4)
	;
    else
    af=(a+4)
	;
	if(b>=119)
    bf=(b-26+4)
	;
    else
    bf=(b+4)
	;
	if(c>=119)
    cf=(c-26+4)
	;
    else
    cf=(c+4)
	;
	if(d>=119)
    df=(d-26+4)
	;
    else
    df=(d+4)
	;
	if(e>=119)
    ef=(e-26+4)
	;
    else
    ef=(e+4)
	;
	printf("%c%c%c%C%c",af,bf,cf,df,ef);
	return 0;
}

