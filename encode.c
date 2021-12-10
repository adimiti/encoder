#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const char table64[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/** 
 * @brief Coverts 24 bit integer in 4 butes base 64
 *
 * @param x		24bit integer
 * @param n		number of input bytes
 * @param buf	pointer to four bytes output container
 */
void into64(int32_t x, char buf[4], int n)
{
	div_t dv;
	int i;

	dv.quot = x;
	dv = div(dv.quot, 64);
	buf[3] = (n>2)?table64[dv.rem]:'='; dv = div(dv.quot, 64);
	buf[2] = (n>1)?table64[dv.rem]:'='; dv = div(dv.quot, 64);
	buf[1] = (n>0)?table64[dv.rem]:'='; dv = div(dv.quot, 64);
	buf[0] = (n>0)?table64[dv.rem]:'=';
}

void ito64(int32_t n, char buf[4])
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


void encode(char *dest, const char *src, int src_len)
{
	union {
		int32_t w;
		uint8_t b[4];
	} x;
	int i;
	div_t dv;

	dv = div(src_len, 3);
	x.b[3] = 0;
	for (i=0;i<dv.quot;i++)
	{
		x.b[2] = src[0+i*3];
		x.b[1] = src[1+i*3];
		x.b[0] = src[2+i*3];
		ito64(x.w, &dest[i*4]);
	}
	if (dv.rem)
	{
		x.b[2] = src[0+i*3] * (dv.rem > 0);
		x.b[1] = src[1+i*3] * (dv.rem > 1);
		x.b[0] = src[2+i*3] * (0);
		into64(x.w, &dest[i*4],dv.rem);
	}
}

/** Bonus function that checks if the char is a valid b64 symbol
 * @in character 
 * @return true iff in is a valid base 64 symbol
 */
bool is_b64(char in)
{
	return (in >= 'A' && in <='Z') ||
		(in >= 'a' && in <='z') ||
		(in >= '0' && in <='9') ||
		(in == '+') || (in == '/') || (in == '=');
}

