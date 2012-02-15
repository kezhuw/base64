#include "base64.h"
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <lua.h>
#include <lauxlib.h>

#define lua64_tostring		luaL_checkstring
#define lua64_tolstring		luaL_checklstring
#define lua64_touserdata(L, i)	luaL_checkudata((L), (i), g_umetatable_name)

const char g_umetatable_name[] = "__base64_userdata_metatable_name__";

typedef void (*EncodeFunc_t)(const char *src, size_t len, char *dst);
typedef long (*DecodeFunc_t)(const char *enc, size_t len, char *dst, size_t *dlen);

static int
_common_encode(lua_State *L, EncodeFunc_t encode) {
	size_t len;
	const char *str = lua64_tolstring(L, 1, &len);
	size_t dlen = base64_encode_length(len);
	luaL_Buffer b;
	char *dst = luaL_buffinitsize(L, &b, dlen);
	encode(str, len, dst);
	luaL_pushresultsize(&b, dlen);
	return 1;
}

static int
_common_decode(lua_State *L, DecodeFunc_t decode) {
	size_t len;
	const char *str = lua64_tolstring(L, 1, &len);
	size_t tlen = base64_decode_length(len);
	luaL_Buffer b;
	char *dst = luaL_buffinitsize(L, &b, tlen);
	size_t dlen;
	if (decode(str, len, dst, &dlen) < 0) {
		lua_pushnil(L);
	} else {
		luaL_pushresultsize(&b, dlen);
	}
	lua_pushnil(L);
	return 2;
}

static int
_encode(lua_State *L) {
	return _common_encode(L, base64_encode);
}

static int
_decode(lua_State *L) {
	return _common_decode(L, base64_decode);
}

static int
_urlsafe_encode(lua_State *L) {
	return _common_encode(L, base64_urlsafe_encode);
}

static int
_urlsafe_decode(lua_State *L) {
	return _common_decode(L, base64_urlsafe_decode);
}

static bool
_string_unique(const char *str, size_t len) {
	size_t i;
	for (i=0; i<len-1; i++) {
		size_t j;
		for (j=i+1; j<len; j++) {
			if (str[j] == str[i]) {
				return 0;
			}
		}
	}
	return 1;
}

static int
_scheme_create(lua_State *L) {
	size_t len;
	const char *str = lua64_tolstring(L, 1, &len);
	size_t padding_len;
	const char *padding = lua64_tolstring(L, 2, &padding_len);
	luaL_argcheck(L, len >= 64, 1, "encoding alphabet must have 64 characters");
	luaL_argcheck(L, _string_unique(str, 64), 1, "encoding alphabet has redundency");
	luaL_argcheck(L, padding_len > 0, 2, "must specify padding character");
	luaL_argcheck(L, memchr(str, padding[0], len) == NULL, 2, "padding reside in alphabet");
	struct base64_scheme *sch = lua_newuserdata(L, base64_scheme_length());
	base64_scheme_launch(sch, str, padding[0]);
	luaL_setmetatable(L, g_umetatable_name);
	return 1;
}

static int
_scheme_encode(lua_State *L) {
	struct base64_scheme *sch = lua64_touserdata(L, 1);
	size_t len;
	const char *str = lua64_tolstring(L, 2, &len);
	size_t dlen = base64_encode_length(len);
	luaL_Buffer b;
	char *dst = luaL_buffinitsize(L, &b, dlen);
	base64_scheme_encode(sch, str, len, dst);
	luaL_pushresultsize(&b, dlen);
	return 1;
}

static int
_scheme_decode(lua_State *L) {
	struct base64_scheme *sch = lua64_touserdata(L, 1);
	size_t len;
	const char *str = lua64_tolstring(L, 2, &len);
	size_t tlen = base64_decode_length(len);
	luaL_Buffer b;
	char *dst = luaL_buffinitsize(L, &b, tlen);
	size_t dlen;
	if (base64_scheme_decode(sch, str, len, dst, &dlen) < 0) {
		lua_pushnil(L);
	} else {
		luaL_pushresultsize(&b, dlen);
	}
	lua_pushnil(L);
	return 2;
}

static int
_scheme_stdmode(lua_State *L) {
	const struct base64_scheme *sch = base64_stdmode_scheme;
	lua_pushlightuserdata(L, (void *)sch);
	luaL_setmetatable(L, g_umetatable_name);
	return 1;
}

static int
_scheme_urlsafe(lua_State *L) {
	const struct base64_scheme *sch = base64_urlsafe_scheme;
	lua_pushlightuserdata(L, (void *)sch);
	luaL_setmetatable(L, g_umetatable_name);
	return 1;
}

static const luaL_Reg fregs[] = {
	{"encode", _encode},
	{"decode", _decode},
	{"urlsafe_encode", _urlsafe_encode},
	{"urlsafe_decode", _urlsafe_decode},
	{"scheme_create", _scheme_create},
	{"scheme_stdmode", _scheme_stdmode},
	{"scheme_urlsafe", _scheme_urlsafe},
	{NULL, NULL}
};

// Methods for scheme
static const luaL_Reg mregs[] = {
	{"encode", _scheme_encode},
	{"decode", _scheme_decode},
	{NULL, NULL}
};

static void
create_metatable(lua_State *L) {
	luaL_newmetatable(L, g_umetatable_name);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, mregs, 0);
	lua_pop(L, 1);
}

LUAMOD_API int
luaopen_base64(lua_State *L) {
	create_metatable(L);
	luaL_newlib(L, fregs);
	return 1;
}
