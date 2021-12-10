#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
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

int fd_out;
int cb(char c[4] , bool done)
{
	if (c)
	{
		write(fd_out, c, 4);
	}
	if (done) printf(" done\n");
	return 0;
}

int compare_files(int fda, const char *ref)
{
	int na;
	char buffa[64];

	lseek(fda, 0, SEEK_SET);
	do {
		na = read(fda, buffa, sizeof(buffa));
		if (na) printf("[%.*s] Output len = %d\n[%s] reference\n", na, buffa, na, ref);
		if (memcmp(buffa, ref, na))
			return 1;
		ref += na;
	} while (na);
	return 0;
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

	puts("Phase one.");
	for (i=0;i<sizeof(tv)/sizeof(*tv);i++)
	{
		test(tv[i].in, tv[i].out, strlen(tv[i].in));
	}
	puts("Phase two.");
	for (i=0;i<sizeof(tv2)/sizeof(*tv2);i++)
	{
		bzero(buf, sizeof(buf));
		encode(buf, tv2[i].in, strlen(tv2[i].in));
		printf("[%s] --> [%s] exp. [%s] diff %d [%s]\n",tv2[i].in, buf, tv2[i].out, strncmp(buf, tv2[i].out,
					strlen(tv2[i].out)),strncmp(buf, tv2[i].out, strlen(tv2[i].out))?REPORT_NOK:REPORT_OK);
	}
#define FILENAME_IN "data.in"
#define FILENAME_OUT "data.out"	

	puts("Phase three.");
	for (i=0;i<sizeof(tv2)/sizeof(*tv2);i++)
	{
		int fd_in;
		fd_in  = open(FILENAME_IN, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		fd_out = open(FILENAME_OUT, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		ftruncate(fd_in, 0);
		ftruncate(fd_out, 0);
		int w = write(fd_in, tv2[i].in, strlen(tv2[i].in));
		lseek(fd_in, 0, SEEK_SET);
		lseek(fd_out, 0, SEEK_SET);
		int n = stream_enc(fd_in, cb);
		int res = compare_files(fd_out, tv2[i].out);
		close(fd_in);
		close(fd_out);
		printf("end of test %d. Total input %d bytes. Test len = %d Result %d %s\n", i, n, w, res, res?REPORT_NOK:REPORT_OK);
		unlink(FILENAME_IN);
		unlink(FILENAME_OUT);
	}
	return 0;
}
