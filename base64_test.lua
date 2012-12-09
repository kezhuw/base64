local base64 = require "base64"

local map = "1qaz2wsx3edc4rfvbgt56yhnmju78ik,.lo90p;/'[-Q#$Y*&XM<>N~`ASRTOP()\""
local pad = "!"

local test = {}
test[#test+1] = "base64"
test[#test+1] = "o13=c./4230c-a]\'"
test[#test+1] = map
test[#test+1] = "$c*.<[}|?'\"t}{]-=IOUE4#"
test[#test+1] = "WkdSa1hISmtaR1JrWEhKY2JtUmtaR1JjYm1Oall3bz0="
test[#test+1] = "WjBHUjNYSDRKYWtaR1JYSEpjYm1Sa1pHUmNibU5qWXdvPTkw"
test[#test+1] = "bGVhc3VyZS4="
test[#test+1] = "28c930-acgoh.r>;']\\rt"
test[#test+1] = "cidsfaooewo412843c13-534209"
test[#test+1] = "MjhjOTMwLWFjZ29oLnI-OyddXHJ0"

-- API usage, beg:{
for i=1,#test do
	local rawstr = test[i]
	local encstr = base64.encode(rawstr)
	local decstr = base64.decode(encstr)
	if rawstr ~= decstr then
		local outstr = string.format("base64.decode(%q) => %q, expect %q", encstr, decstr, rawstr)
		print(outstr)
	end

	local rawstr = test[i]
	local encstr = base64.urlsafe_encode(rawstr)
	local decstr = base64.urlsafe_decode(encstr)
	if rawstr ~= decstr then
		local outstr = string.format("base64.urlsafe_decode(%q) => %q, expect %q", encstr, decstr, rawstr)
		print(outstr)
	end

	local schtab = {}
	schtab[1] = base64.scheme_stdmode()
	schtab[2] = base64.scheme_urlsafe()
	schtab[3] = base64.scheme_create(map, pad)

	for i=1,#schtab do
		local sch = schtab[i]
		local rawstr = test[i]
		local encstr = sch:encode(rawstr)
		local decstr = sch:decode(encstr)
		if rawstr ~= decstr then
			local outstr = string.format("sch:decode(%q) => %q, expect %q", encstr, decstr, rawstr);
			print("")
			print(outstr)
			if sch == schtab[1] then
				print("Using scheme_stdmode")
			end
			if sch == schtab[2] then
				print("Using scheme_urlsafe")
			end
			if sch == schtab[3] then
				print(string.format("Using map %q padding %q", map, pad))
			end
			print("")
		end
	end
end
-- API usage, end.}

print("TEST PASSED")
