#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "scheme.c"

static const char std[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char url[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

int
main(void) {
	assert(strlen(std) == 64);

	FILE *file = fopen("mode64.c", "w");
	fprintf(file, "#ifndef __MODE64_C\n#define __MODE64_C\n");
	fprintf(file, "\n#include \"scheme.c\"\n");

	struct base64_scheme stdmode;
	base64_scheme_launch(&stdmode, std, '=');
	fprintf(file, "\nconst struct base64_scheme base64_stdmode_scheme = ");
	base64_scheme_output(&stdmode, file);

	struct base64_scheme urlsafe;
	base64_scheme_launch(&urlsafe, url, '=');
	fprintf(file, "\nconst struct base64_scheme base64_urlsafe_scheme = ");
	base64_scheme_output(&urlsafe, file);
	fprintf(file, "#endif\n");
	return 0;
}
