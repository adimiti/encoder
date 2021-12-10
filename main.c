
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#include "encode.h"
 

 
int main(void)
{
	char buf[100] = {0xA5, 0xA5, 0xA5, 0xA5, 0xA5};
	int x;
	int res;

	x = 'M';
	x <<= 8;
	x |= 'a';
	x <<= 8;
	x |= 'n';

	ito64(x, buf);
	res = strncmp(buf, "TWFu", 4);
	printf("[%4.4s], expected [%s], diff %d [%s]\n", buf, "TWFu", res, (res?"NOK":"OK"));

	return 0;
}
