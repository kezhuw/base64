#ifndef __SCHEME_C
#define __SCHEME_C
struct base64_scheme {
	char encmap[64];	// [64] == '='
	char padding;
	char decmap[256];
};

size_t
base64_scheme_length(void) {
	return sizeof(struct base64_scheme);
}

void
base64_scheme_launch(struct base64_scheme *sch, const char *map, char padding) {
	for (size_t i=0; i<256; i++) {
		sch->decmap[i] = 0xFF;
	}
	for (size_t i=0; i<64; i++) {
		sch->decmap[map[i]] = (char)i;
	}
	memcpy(sch->encmap, map, 64);
	sch->padding = padding;
}

#include <stdio.h>
void
base64_scheme_output(struct base64_scheme *sch, FILE *file) {
	fprintf(file, "{\n");

	fprintf(file, "\t{");
	for (size_t i=0; i<64; i++) {
		long newline = (i%10 == 0);
		if (newline) {
			if (i != 0) fprintf(file, ",");
			fprintf(file, "\n\t\t");
		} else {
			fprintf(file, ", ");
		}
		fprintf(file, "'%c'", sch->encmap[i]);
	}
	fprintf(file, "\n\t},\n");

	fprintf(file, "'%c',\n", sch->padding);

	fprintf(file, "\t{");
	for (size_t i=0; i<256; i++) {
		long newline = (i%10 == 0);
		if (newline) {
			if (i != 0) {
				fprintf(file, ",");
			}
			fprintf(file, "\n\t\t");
		} else {
			fprintf(file, ", ");
		}
		fprintf(file, "0x%02X", (unsigned char)sch->decmap[i]);
	}
	fprintf(file, "\n\t}\n");

	fprintf(file, "};\n");
}
#endif
