#ifndef __BASE64_H
#define __BASE64_H
#include <stddef.h>

// Return exactly length of encoded data including trailing paddings
// for n bytes of source data.
size_t base64_encode_length(size_t n);

// Return maximum decoded length for n bytes of encoded data.
size_t base64_decode_length(size_t n);

// For *_encode()/*_decode(), caller must guarantee sufficient room for writing.
static inline void base64_encode(const char *src, size_t len, char *dst);

// For *_decode():
// Return a negative for errors, store to *dlen actual size of decoded data written to dst.
//
// -1 => data corruption, *dlen undefined.
// 0 => success.
// 1 => encounter padding character in middle of enc.
static inline long base64_decode(const char *enc, size_t len, char *dst, size_t *dlen);
static inline void base64_urlsafe_encode(const char *src, size_t len, char *dst);
static inline long base64_urlsafe_decode(const char *enc, size_t len, char *dst, size_t *dlen);


struct base64_scheme;

// Return a size for allocating memory to store a 'struct base64_scheme'.
size_t base64_scheme_length(void);

// Initialize *sch scheme.
//
// Caller must guarantee:
//   map has 64 characters and no repetition;
//   padding character can't reside in map.
void base64_scheme_launch(struct base64_scheme *sch, const char *map, char pad);
void base64_scheme_encode(const struct base64_scheme *sch, const char *src, size_t len, char *dst);
long base64_scheme_decode(const struct base64_scheme *sch, const char *enc, size_t len, char *dst, size_t *dlen);


// _stdmode_ use the standard base64 alphabet.
// _urlsafe_ use the URL and Filename safe base64 alphabet.
const struct base64_scheme *base64_stdmode_scheme;
const struct base64_scheme *base64_urlsafe_scheme;

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
