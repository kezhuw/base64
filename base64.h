#ifndef __BASE64_H
#define __BASE64_H
#include <stddef.h>

size_t base64_encode_length(size_t srclen);
size_t base64_decode_length(size_t enclen);

const struct base64_scheme *base64_stdmode_scheme;
const struct base64_scheme *base64_urlsafe_scheme;

size_t base64_scheme_length(void);
void base64_scheme_launch(struct base64_scheme *sch, const char *map, char pad);
void base64_scheme_encode(const struct base64_scheme *sch, const char *src, size_t len, char *dst);
long base64_scheme_decode(const struct base64_scheme *sch, const char *enc, size_t len, char *dst, size_t *dlen);

#define base64_stdmode_scheme	((const struct base64_scheme *)&base64_stdmode_scheme)
#define base64_urlsafe_scheme	((const struct base64_scheme *)&base64_urlsafe_scheme)

static inline void
base64_encode(const char *src, size_t len, char *dst) {
	base64_scheme_encode(base64_stdmode_scheme, src, len, dst);
}

static inline long
base64_decode(const char *enc, size_t len, char *dst, size_t *dlen) {
	return base64_scheme_decode(base64_stdmode_scheme, enc, len, dst, dlen);
}

static inline void
base64_urlsafe_encode(const char *src, size_t len, char *dst) {
	base64_scheme_encode(base64_urlsafe_scheme, src, len, dst);
}

static inline long
base64_urlsafe_decode(const char *enc, size_t len, char *dst, size_t *dlen) {
	return base64_scheme_decode(base64_urlsafe_scheme, enc, len, dst, dlen);
}
#endif
