#include <stddef.h>
#include <string.h>
#include "scheme.c"
#include "mode64.c"

size_t
base64_encode_length(size_t srclen) {
	return ((srclen+2)/3)*4;
}

size_t
base64_decode_length(size_t enclen) {
	return ((enclen+3)/4)*3;
}

void
base64_scheme_encode(const struct base64_scheme *sch, const char *src, size_t len, char *dst) {
	if (len == 0)
		return;
	const unsigned char *raw = (void *)src;
	for (;;) {
		unsigned long d0, d1, d2, d3;
		switch (len) {
		case 1: {
			unsigned long s0 = raw[0];
			d0 = s0 >> 2;
			d1 = (s0 & 0x03) << 4;
			d2 = 64;
			d3 = 64;
		} break;
		case 2: {
			unsigned long s0 = raw[0], s1 = raw[1];
			d0 = s0 >> 2;
			d1 = (s0 & 0x03) << 4;
			d1 |= s1 >> 4;
			d2 = (s1 & 0x0F) << 2;
			d3 = 64;
		} break;
		default: {
			unsigned long s0 = raw[0], s1 = raw[1], s2 = raw[2];
			d0 = s0 >> 2;
			d1 = ((s0 << 4) | (s1 >> 4)) & 0x3F;
			d2 = ((s1 << 2) | (s2 >> 6)) & 0x3F;
			d3 = s2 & 0x3F;
		} break;
		}
		dst[0] = sch->encmap[d0];
		dst[1] = sch->encmap[d1];
		dst[2] = sch->encmap[d2];
		dst[3] = sch->encmap[d3];
		if (len <= 3)
			break;
		len -= 3;
		raw += 3;
		dst += 4;
	}
}

long
base64_scheme_decode(const struct base64_scheme *sch, const char *enc, size_t len, char *dst, size_t *plen) {
	const unsigned char *src = (void *)enc;
	const char *bak = dst;
	long padding = sch->padding;
	for (;;) {
		if (len < 2) {
			return -1;
		}
		long d0 = sch->decmap[src[0]], d1 = sch->decmap[src[1]];
		if (d0 >= 64 || d1 >= 64) {
			return -1;
		}
		long n = 2;
		long d2=0, d3=0;
		if (len > 2) {
			long t = src[2];
			if (t != padding) {
				d2 = sch->decmap[t];
				n = 3;
				if (len > 3) {
					t = src[3];
					if (t != padding) {
						d3 = sch->decmap[t];
						n = 4;
					}
				}
			}
		}
		if (d2 >= 64 || d3 >= 64) {
			return -1;
		}
		switch (n) {
		case 4:
			dst[2] = (char)(d2<<6 | d3);
		case 3:
			dst[1] = (char)(d2>>2 | d1<<4);
		case 2:
			dst[0] = (char)(d1>>4 | d0<<2);
		}
		dst += n-1;
		if (n < 4 && len > 4) {
			*plen = (size_t)(dst-bak);
			return 1;
		}
		if (len <= 4)
			break;
		len -= 4;
		src += 4;
	}
	*plen = (size_t)(dst-bak);
	return 0;
}
