#include <stdlib.h>

static const char table64[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/** 
 * @brief Coverts 24 bit integer in 4 butes base 64
 *
 * @param n		24bit integer
 * @param buf	pointer to four bytes output container
 */
void ito64(int n, char buf[4])
{
	div_t dv;
	int i;

	dv.quot = n;
	for (i=0;i<4;i++)
	{
		dv = div(dv.quot, 64);
		buf[3-i] = table64[dv.rem];
	}
}














