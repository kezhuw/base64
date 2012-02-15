local base64 = require "base64"

local encoded = base64.encode("base64")
assert(base64.decode(encoded) == "base64")

local sch = base64.scheme_urlsafe()

local map = "1qaz2wsx3edc4rfvbgt56yhnmju78ik,.lo90p;/'[-Q#$Y*&XM<>N~`ASRTOP()\""
local def = base64.scheme_create(map, "!")

local raw = "$c*.<[}|?'\"t}{]-=IOUE4#"
local enc = def:encode(raw)
assert((def:decode(enc)) == raw)
