#include "base64.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

struct pair {
	const char *rawstring;
	const char *stdencode;
	const char *urlencode;
};

#include "base64_test.h"

static void
TestStdmodeEncode(void) {
	for (const struct pair *p = pairs; p->rawstring != NULL; p++) {
		size_t len = strlen(p->rawstring);
		size_t dlen = base64_encode_length(len);
		char chs[dlen+1];
		base64_encode(p->rawstring, len, chs);
		chs[dlen] = '\0';
		if (strcmp(p->stdencode, chs) != 0) {
			fprintf(stderr, "stdmode_encode(%s) => %s, expect %s\n", p->rawstring, chs, p->stdencode);
			abort();
		}
	}
}

static void
TestStdmodeDecode(void) {
	for (const struct pair *p = pairs; p->stdencode != NULL; p++) {
		size_t len = strlen(p->stdencode);
		size_t mlen = base64_decode_length(len);
		char chs[mlen+1];
		size_t dlen;
		if (base64_decode(p->stdencode, len, chs, &dlen) < 0) {
			fprintf(stderr, "stdmode_decode(%s) fail! expect %s\n", p->stdencode, p->rawstring);
			abort();
		}
		chs[dlen] = '\0';
		if (strcmp(p->rawstring, chs) != 0) {
			fprintf(stderr, "stdmode_decode(%s) => %s, expect %s\n", p->stdencode, chs, p->rawstring);
			abort();
		}
	}
}

static void
TestUrlsafeEncode(void) {
	for (const struct pair *p = pairs; p->rawstring != NULL; p++) {
		size_t len = strlen(p->rawstring);
		size_t dlen = base64_encode_length(len);
		char chs[dlen+1];
		base64_urlsafe_encode(p->rawstring, len, chs);
		chs[dlen] = '\0';
		if (strcmp(p->urlencode, chs) != 0) {
			fprintf(stderr, "urlsafe_encode(%s) => %s, expect %s\n", p->rawstring, chs, p->urlencode);
			abort();
		}
	}
}

static void
TestUrlsafeDecode(void) {
	for (const struct pair *p = pairs; p->urlencode != NULL; p++) {
		size_t len = strlen(p->urlencode);
		size_t mlen = base64_decode_length(len);
		char chs[mlen+1];
		size_t dlen;
		long ret;
		if ((ret = base64_urlsafe_decode(p->urlencode, len, chs, &dlen)) < 0) {
			fprintf(stderr, "urlsafe_decode(%s) fail(%ld)! expect %s\n", p->urlencode, ret, p->rawstring);
			abort();
		}
		chs[dlen] = '\0';
		if (strcmp(p->rawstring, chs) != 0) {
			fprintf(stderr, "urlsafe_decode(%s) => %s, expect %s\n", p->urlencode, chs, p->rawstring);
			abort();
		}
	}
}

static void
TestScheme(void) {
	const char *map = "1qaz2wsx3edc4rfvbgt56yhnmju78ik,.lo90p;/'[-Q#$Y*&XM<>N~`ASRTOP()\"";
	int padding = '!';
	struct base64_scheme *sch = malloc(base64_scheme_length());
	base64_scheme_launch(sch, map, padding);
	const char *raw = "$c*.<[}|?'\"t}{]-=IOUE4#";
	size_t rawlen = strlen(raw);
	size_t enclen = base64_encode_length(rawlen);
	char enc[enclen+1];
	base64_scheme_encode(sch, raw, rawlen, enc);
	enc[enclen] = '\0';
	char dec[base64_decode_length(enclen)+1];
	size_t declen;
	base64_scheme_decode(sch, enc, enclen, dec, &declen);
	dec[declen] = '\0';
	if (strcmp(raw, dec) != 0) {
		fprintf(stderr, "MAP:\n%s\n\n",  map);
		fprintf(stderr, "raw:\n%s\nenc:\n%s\ndec:\n%s\n", raw, enc, dec);
		abort();
	}
}

int
main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	TestStdmodeEncode();
	TestStdmodeDecode();
	TestUrlsafeEncode();
	TestUrlsafeDecode();
	TestScheme();
	puts("TEST PASSED");
	return 0;
}
