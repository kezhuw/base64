package main

import (
	"encoding/base64"
	"fmt"
	"os"
)

type pair struct {
	rawstring, stdencode, urlencode string
}

var pairs = []pair {
	{"ZGRkXHJkZGRkXHJcbmRkZGRcbmNjYwo=", "", ""},
	{"Z0GR3XH4JakZGRXHJcbmRkZGRcbmNjYwo=90", "", ""},
	{"pleasure.", "", ""},
	{"leasure.", "", ""},
	{"4x24sdfcm432-c", "", ""},
	{"cciowrqorepppac", "", ""},
	{"28c930-acgoh.r>;']\\rt", "", ""},
	{"cidsfaooewo412843c13-534209", "", ""},
	{"$c*.<[}|?'\"t}{]-=IOUE4#", "", ""},
}

func main() {
	for i := 0; i<len(pairs); i++ {
		pairs[i].stdencode = base64.StdEncoding.EncodeToString([]byte(pairs[i].rawstring))
		pairs[i].urlencode = base64.URLEncoding.EncodeToString([]byte(pairs[i].rawstring))
	}

	file, _ := os.OpenFile("base64_test.h", os.O_WRONLY | os.O_CREATE, 0666)
	fmt.Fprintf(file, "static const struct pair pairs[] = {\n")
	for _, p := range pairs {
		fmt.Fprintf(file, "{%q, %q, %q},\n", p.rawstring, p.stdencode, p.urlencode)
	}
	fmt.Fprintf(file, "{NULL, NULL, NULL},\n");
	fmt.Fprintf(file, "};\n")
	file.Close()
}
