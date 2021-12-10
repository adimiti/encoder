#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#include "encode.h"
 
#define REPORT_NOK	"\033[0;41mNok\033[0m"
#define REPORT_OK	"\033[0;42mOk\033[0m"

int test(const char a[3], const char b[4], int n)
{
	int x;
	int res;
	char buf[5];

	bzero(buf, sizeof(buf));

	x = a[0];
	x <<= 8;
	x |= a[1];
	x <<= 8;
	x |= a[2];
	into64(x, buf, n);
	res = strncmp(buf, b, 4);
	printf("[%3.3s] --> [%4.4s], expected [%s], diff %d [%s]\n", a, buf, b, res, (res?REPORT_NOK:REPORT_OK));

	return res;
}

 
int main(void)
{
	static const struct
	{
		char in[4];
		char out[5];
	} tv[] =
	{
		{"Man", "TWFu"},
		{"1", "MQ=="},
		{"12", "MTI="},
		{"123", "MTIz"},
	};
	static const struct
	{
		char *in;
		char *out;
	} tv2[] =
	{
		{"The quick brown fox jumps over the lazy dog", "VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw=="},
		{"The quick brown fox jumps over the lazy dog.", "VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZy4="},
		{"Many hands make light work.", "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu"},
		{"1", "MQ=="},
		{"12", "MTI="},
		{"123", "MTIz"},
	};
	int i;
	char buf[4096];

	for (i=0;i<sizeof(tv)/sizeof(*tv);i++)
	{
		test(tv[i].in, tv[i].out, strlen(tv[i].in));
	}
	for (i=0;i<sizeof(tv2)/sizeof(*tv2);i++)
	{
		bzero(buf, sizeof(buf));
		encode(buf, tv2[i].in, strlen(tv2[i].in));
		printf("[%s] --> [%s] exp. [%s] diff %d [%s]\n",tv2[i].in, buf, tv2[i].out, strncmp(buf, tv2[i].out,
					strlen(tv2[i].out)),strncmp(buf, tv2[i].out, strlen(tv2[i].out))?REPORT_NOK:REPORT_OK);
	}


	return 0;
}
